import socket
import struct
import threading
import queue

from .device import *
from .packet import *


CDDP_CONN_TIMEOUT    = 1000
CDDP_CONNACK_TIMEOUT = 10000
CDDP_RECEIVE_TIMEOUT = 10
CDDP_TASK_TICKRATE   = 100


def struct_to_bytes(data: ctypes.Structure, size: int) -> bytes:
    return (ctypes.c_uint8 * size)(*list(bytearray(data)))


class Server:

    def __init__(self, host, port, loop=None):
        # parameters
        self.host = host
        self.port = port

        # Queues
        self.conn_queue = queue.Queue()
        self.input_queue = queue.Queue()
        self.output_queue = queue.Queue()

        # connected devices
        self.device_address = 1

        self.devices = {}
        self.conn_pool = []

    # public methods

    def run(self):

        # local variables

        # serve
        self.serve()

    def serve(self):
        # local variables

        # open server socket and start server coroutine
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((self.host, self.port))

        # start worker tasks
        conn_thread = threading.Thread(target=self.conn_worker)
        input_thread = threading.Thread(target=self.input_worker)
        output_thread = threading.Thread(target=self.output_worker)
        packet_thread = threading.Thread(target=self.packet_worker)

        conn_thread.start()
        input_thread.start()
        output_thread.start()
        packet_thread.start()

        # handle new incoming connections
        print("Server starting")

        # Main loop
        should_quit = False
        while not should_quit:
            # accept new connection
            server_socket.listen(1)
            conn, addr = server_socket.accept()

            # handle new connection asynchronously
            self.conn_queue.put((conn, addr))

        server_socket.close()

        conn_thread.join()
        input_thread.join()
        output_thread.join()
        packet_thread.join()

    def conn_worker(self):
        # local variables

        # handle connections from connection queue
        print("Conn Worker starting")

        # main loop
        should_quit = False
        while not should_quit:
            # clear new connection queue
            while not self.conn_queue.empty():
                (conn, addr) = self.conn_queue.get()

                device_address = self.new_device_address()
                device = Device(device_address)

                if self.new_device_handshake(conn, device):
                    # if device performs handshake, start the task and add it to the connected devices
                    self.devices[device_address] = device
                    self.conn_pool.append(conn)
                else:
                    # if handshake fails, close connection
                    conn.close()

    def input_worker(self):
        # local variables

        # handle incoming data from connection pool
        print("Input Worker starting")

        # main loop
        should_quit = False
        while not should_quit:
            # check each connection
            for conn in self.conn_pool:
                # read from connection
                pkt = self.receive(conn, CDDP_RECEIVE_TIMEOUT)

                if pkt:
                    # if a packet was received, put it into the processing queue
                    self.input_queue.put(pkt)

        self.input_queue = None

    def packet_worker(self):
        # local variables

        # handle packets from incoming queue
        print("Packet Worker starting")

        # input queue -> update devices/database -> output queue
        # main loop
        should_quit = False
        while not should_quit:
            # empty input queue
            while not self.input_queue.empty():
                pkt = self.input_queue.get()
                self.handle_packet(pkt)

    def output_worker(self):
        # local variables

        # send packets from outgoing queue
        print("Output Worker starting")

        # main loop
        should_quit = False
        while not should_quit:
            # clear the outgoing queue
            while not self.output_queue.empty():
                # send all outgoing packets

                pkt = self.output_queue.get()
                self.send(pkt)

    def new_device_handshake(self, conn, device, retry=1000, conn_pkt=None):
        # coroutine may run for a bit while waiting for a handshake attempt

        if conn_pkt:
            # if the device has already connected and is sending a new interface
            # use the passed conn packet
            pkt = conn_pkt

            # clear the device from memory, and reset the device
            device_addr = device.address
            self.devices.pop(device_addr)
            device.__init__(device_addr)

        else:
            # if this is a new connection, wait for CONN packet
            attempt = 0
            pkt = self.receive(conn, CDDP_CONN_TIMEOUT)
            while pkt.id != DataID.CDDP_SYS_DATA_CONN.value and attempt < retry:
                # ignore other packets
                pkt = self.receive(conn, CDDP_CONN_TIMEOUT)
                attempt = attempt + 1

        if pkt.id == DataID.CDDP_SYS_DATA_CONN.value:
            # if valid CONN packet has been acquired, get the connection data
            conn_data = ConnData.from_buffer_copy(pkt.buf)

            # create first CONNACK packet
            connack_data = ConnackData()
            connack_data.addr = device.address
            connack_data.intrf_count = -1

            connack_pkt = Packet()

            connack_pkt.addr = device.address
            connack_pkt.id   = DataID.CDDP_SYS_DATA_CONNACK.value
            connack_pkt.tick = 0
            connack_pkt.seq  = 0
            connack_pkt.data = connack_data.buf

            # send CONNACK packet
            self.send(connack_pkt)

            # read all incoming interface packets
            count = 0
            while count < conn_data.intrf_count and pkt:
                # read as many interface packets as indicated in CONN pkt
                pkt = self.receive(conn, CDDP_CONN_TIMEOUT)
                if pkt and pkt.id == DataID.CDDP_SYS_DATA_INTRF.value:
                    # if it's an interface packet, get interface data
                    intrf_data = IntrfData.from_buffer_copy(pkt.buf)

                    # update device
                    device.interface[pkt.id].data = intrf_data

                    # continue
                    count = count + 1

            # create the second CONNACK packet
            connack_data = ConnackData()
            connack_data.addr = device.address
            connack_data.intrf_count = count

            connack_pkt.data = connack_data.buf

            # send CONNACK packet
            self.send(connack_pkt)

            # flag device as connected
            device.handshook = True

        return device.handshook

    def send(self, conn: socket, pkt: Packet):

        # local variables
        rc = 1

        total = 0
        while total < CDDP_PKT_SIZE:
            # Send pkt until all has been transmitted
            sent = conn.send(bytes(pkt.buf)[bytes_sent:])
            if sent:
                total = total + sent
            else:
                rc = 0
                break

        return rc

    def receive(self, conn, timeout=0) -> Packet:

        if timeout:
            conn.settimeout(timeout)

        try:
            pkt_buf = conn.recv(CDDP_PKT_SIZE)
            pkt = Packet.from_buffer_copy(pkt_buf)

            print(f"Received packet: {bytes(pkt)}")
            print(f"Packet id: {pkt.id}")
        except:
            pkt = None

        if timeout:
            conn.settimeout(0)

        return pkt

    def handle_packet(self, conn: socket.socket, pkt: Packet) -> int:
        # local variables
        rc = 0

        if pkt.id in range(DataID.CDDP_DATA_ID_FIRST.value, DataID.CDDP_DATA_ID_LAST.value):
            # if valid packet id, check if device has connected
            device = self.devices.get(pkt.addr)
            if device:
                # if the device has connected, handle the packet

                if pkt.id == DataID.CDDP_SYS_DATA_CONN.value:
                    self.new_device_handshake(conn, device, conn_pkt=pkt)
                    self.devices[device.address] = device
                    pass
                elif pkt.id == DataID.CDDP_SYS_DATA_CONNACK.value:
                    # Server should not receive this value, error
                    rc = -1
                    pass
                elif pkt.id == DataID.CDDP_SYS_DATA_KICK.value:
                    # Server should not receive this value, error
                    rc = -1
                    pass
                elif pkt.id == DataID.CDDP_SYS_DATA_DISCONN.value:
                    pass
                # check for SNSR packets
                elif pkt.id == DataID.CDDP_SNSR_DIST_ID.value:
                    # update interface data
                    device.interface[pkt.id].pkt = pkt
                    rc = 1
                    pass

        return rc

    def new_device_address(self):
        self.device_address = self.device_address + 1
        return self.device_address
