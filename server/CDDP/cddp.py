import asyncio
import enum
import socket
import ctypes

# constants

CDDP_DATA_SIZE       = 64
CDDP_PKT_SIZE        = 128
CDDP_CONN_TIMEOUT    = 1000
CDDP_CONNACK_TIMEOUT = 10000
CDDP_TASK_TIMEOUT    = 100
CDDP_TASK_TICKRATE   = 100


class DataID(enum.Enum):
    CDDP_DATA_ID_FIRST = 0

    # System data IDs are reserved IDs used to handle module configuration and connecting to the hub
    CDDP_SYS_DATA_ID_FIRST = CDDP_DATA_ID_FIRST

    CDDP_SYS_DATA_CONN     = CDDP_SYS_DATA_ID_FIRST
    CDDP_SYS_DATA_CONNACK  = 1
    CDDP_SYS_DATA_KICK     = 2
    CDDP_SYS_DATA_DISCONN  = 3

    CDDP_SYS_DATA_ID_LAST = 9

    # Sensor IDs are used to store and send data to connected sensors by the sensor module
    CDDP_SNSR_ID_FIRST = 10

    CDDP_SNSR_DIST_ID = CDDP_SNSR_ID_FIRST

    CDDP_SNSR_ID_LAST = 19

    CDDP_RES_ID_FIRST = 92                      # 99 through 99 are reserved because only 192 bits are available to send the interface mask
    CDDP_RES_ID_LAST = 99                       # so the simulated IDs would not be able to replicate some embedded IDs

    # Simulated Data IDs are used to simulate embedded functions, they are the embedded ID counterpart offset by 100
    CDDP_SIM_DATA_ID_FIRST = 100

    CDDP_SIM_SNSR_ID_FIRST = CDDP_SIM_DATA_ID_FIRST

    CDDP_SIM_SNSR_DIST_ID = 110,

    CDDP_SIM_DATA_ID_LAST = 192

    CDDP_DATA_ID_LAST = CDDP_SIM_DATA_ID_LAST

    CDDP_SYS_DATA_ID_COUNT = 10
    CDDP_SIM_DATA_ID_COUNT = 93
    CDDP_DATA_ID_COUNT     = 192


# types

class Packet(ctypes.Structure):
    _fields_ = [
        ("addr", ctypes.c_uint32),
        ("id",   ctypes.c_uint32),
        ("tick", ctypes.c_uint64),
        ("data", ctypes.POINTER(ctypes.c_uint8)),
        ("buf",  ctypes.POINTER(ctypes.c_uint8))
    ]


class ConnData(ctypes.Structure):
    _fields_ = [
        ("device", ctypes.c_uint8),
        ("intrf",  ctypes.POINTER(ctypes.c_uint8)),
        ("wrtbl",  ctypes.POINTER(ctypes.c_uint8)),
        ("buf",    ctypes.POINTER(ctypes.c_uint8))
    ]

class ConnackData(ctypes.Structure):
    _fields_ = [
        ("addr", ctypes.c_uint32),
        ("buf",  ctypes.POINTER(ctypes.c_uint8))
    ]


class Interface:
    def __init__(self, enabled=True, writable=False, pkt=None):
        self.enabled  = enabled
        self.writable = writable
        self.pkt      = pkt

        # state
        self.updated = False


class Device:

    def __init__(self, conn, address, loop=None):
        self.conn    = conn
        self.address = address
        self.loop    = loop if loop else asyncio.get_event_loop()

        # init state
        self.task      = None
        self.handshook = False
        self.interface = [None] * DataID.CDDP_DATA_ID_COUNT.value

    async def start(self):
        self.task = self.loop.create_task(self.device_tick_task())
        self.loop.run_forever(self.task)

    async def device_tick_task(self):
        # periodically refresh device interface data
        while True:
            await self.refresh()

            asyncio.sleep(CDDP_TASK_TICKRATE/1000)

    async def refresh(self):
        # refresh interface
        await (self.refresh_input(), self.refresh_output())

    async def refresh_input(self):
        # read any data from connection and update self
        pkt = await self.receive(CDDP_TASK_TIMEOUT)
        while pkt:
            self.handle_packet(pkt)

            pkt = await self.receive(CDDP_TASK_TIMEOUT)

    async def refresh_output(self):
        # write any writable interface IDs to the connection
        for inter in self.interface:
            # if it's writable and updated, write new data in packet to connection
            if inter.writable and inter.updated:
                # write to connection
                self.send(inter.pkt)

    async def handshake(self, retry=1000, conn_pkt=None):

        if self.handshook and conn_pkt:
            # if connected device is updating interface
            pkt = conn_pkt

        else:
            # if it's a newly connected device sending first interface, wait for CONN packet
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

            # send CONNACK packet

            # flag device as connected
            self.handshook = True

        return self.handshook

    async def send(self, pkt: Packet):
        pass

    async def receive(self, timeout=0) -> Packet:

        if timeout:
            self.conn.settimeout(timeout)

        pkt_buf = await self.loop.sock_recv(self.conn, CDDP_PKT_SIZE)
        pkt = Packet.from_buffer(pkt_buf)

        if timeout:
            self.conn.settimeout(0)

        return pkt

    def handle_packet(self, pkt):
        if pkt.id in range(DataID.CDDP_DATA_ID_FIRST.value, DataID.CDDP_DATA_ID_LAST.value):
            # if valid packet id, check for SYS packets
            if pkt.id == DataID.CDDP_SYS_DATA_CONN.value:
                pass
            elif pkt.id == DataID.CDDP_SYS_DATA_CONNACK.value:
                pass
            elif pkt.id == DataID.CDDP_SYS_DATA_KICK.value:
                pass
            elif pkt.id == DataID.CDDP_SYS_DATA_DISCONN.value:
                pass
            # check for SNSR packets
            elif pkt.id == DataID.CDDP_SNSR_DIST_ID.value:
                self.interface[pkt.id].pkt = pkt
                pass


class Server:

    def __init__(self, host, port, loop=None):
        # parameters
        self.host = host
        self.port = port
        self.loop = loop if loop else asyncio.get_event_loop()

        # initialize members
        self.address = None
        self.connected = False
        self.socket = None

        # server tasks
        self.server_task = None

        # connected devices
        self.devices_connected = []

    # public methods

    async def start(self):

        # open server socket and start server coroutine
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind((self.host, self.port))

        # start server worker
        self.server_task = self.loop.create_task(self._server_worker())
        self.loop.run_forever(self.server_task)

    async def _server_worker(self):
        # handle new connections

        while True:
            # accept new connection
            self.socket.listen(1)
            conn, addr = await self.loop.sock_accept(self.socket)

            # handle new connection
            self._handle_conn(conn, addr)

    async def _handle_conn(self, conn, addr):
        device_addr = len(self.devices_connected) + 1
        device = Device(conn, device_addr)

        if await device.handshake():
            # if device performs handshake, start the task and add it to the connected devices
            device.start()
            self.devices_connected.append(device)
        else:
            # if handshake fails, close connection
            conn.close()
