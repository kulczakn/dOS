import asyncio
import socket

from device import *
from packet import *


CDDP_CONN_TIMEOUT    = 1000
CDDP_CONNACK_TIMEOUT = 10000
CDDP_TASK_TIMEOUT    = 100
CDDP_TASK_TICKRATE   = 100


class Server:

    def __init__(self, host, port, loop=None):
        # parameters
        self.host = host
        self.port = port

        # initialize members
        self.socket = None

        # server tasks
        self.server_task = None

        # connected devices
        self.device_addr = 1
        self.devices = []
        self.conn_pool = []

    # public methods

    async def start(self):

        # open server socket and start server coroutine
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind((self.host, self.port))

        # start server worker
        self.server_task = asyncio.create_task(self._server_worker())

    async def _server_worker(self):
        # handle new connections
        print("Server Worker starting")

        while True:
            # accept new connection
            self.socket.listen(1)
            conn, addr = await self.loop.sock_accept(self.socket)

            # handle new connection asynchronously
            loop = asyncio.get_running_loop()
            loop.run_until_complete(self._handle_conn(conn, addr))

    async def _handle_conn(self, conn, conn_addr):
        device_addr = len(self.devices) + 1
        device = Device(device_addr)

        if await self._handshake(device):
            # if device performs handshake, start the task and add it to the connected devices
            self.devices[device_addr] = device
            self.conn_pool.append(conn)
        else:
            # if handshake fails, close connection
            conn.close()

    async def _handshake(self, device, retry=1000, conn_pkt=None):
        # coroutine may run for a bit while waiting for a handshake attempt

        if conn_pkt:
            # if the device has already connected and is sending a new interface
            # use the passed conn packet
            pkt = conn_pkt

            # clear the device from memory, and reset the device
            self.devices.remove(device.address)
            device.__init__()

        else:
            # if this is a new connection, wait for CONN packet
            attempt = 0
            pkt = await self.receive(CDDP_CONN_TIMEOUT)
            while pkt.id != DataID.CDDP_SYS_DATA_CONN and attempt < retry:
                # ignore other packets
                pkt = await self.receive(CDDP_CONN_TIMEOUT)
                attempt = attempt + 1

        if pkt.id == DataID.CDDP_SYS_DATA_CONN.value:
            # if valid CONN packet has been acquired
            conn_data = ConnData.from_buffer(pkt.data)

            # check each data id to see which are in the device interface in CONN

            # update local data
            # update device info and interface

            # create CONNACK packet
            connack_data = ConnackData()
            connack_data.addr = device.address

            connack_pkt = Packet()
            connack_pkt.data = bytes(connack_pkt)

            # send CONNACK packet
            await self.send(connack_pkt)

            # flag device as connected
            device.handshook = True

        return device.handshook

    async def send(self, pkt: Packet):
        pass

    async def receive(self, conn, timeout=0) -> Packet:

        if timeout:
            conn.settimeout(timeout)

        loop = asyncio.get_running_loop()

        pkt_buf = await loop.sock_recv(self.conn, CDDP_PKT_SIZE)
        pkt = Packet.from_buffer(pkt_buf)

        if timeout:
            conn.settimeout(0)

        return pkt

    def handle_packet(self, pkt: Packet) -> int:
        # local variables
        rc = 0

        if pkt.id in range(DataID.CDDP_DATA_ID_FIRST.value, DataID.CDDP_DATA_ID_LAST.value):
            # if valid packet id, check if device has connected
            device = self.devices.get(pkt.addr)
            if device:
                # if the device has connected, handle the packet

                if pkt.id == DataID.CDDP_SYS_DATA_CONN.value:
                    return self._handshake(device, conn_pkt=pkt)
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

    def _device_address(self):
        self.device_addr = self.device_addr + 1
        addr = self.device_addr
        return addr
