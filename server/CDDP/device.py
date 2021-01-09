from packet import *


class Interface:
    def __init__(self, enabled=True, writable=False, pkt=None):
        self.enabled  = enabled
        self.writable = writable
        self.pkt      = pkt

        # state
        self.updated = False


class Device:

    def __init__(self, address):
        self.address = address

        # init state
        self.interface = [Interface(), ] * DataID.CDDP_DATA_ID_COUNT
        self.handshook = False
