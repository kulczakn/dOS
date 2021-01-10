from .packet import *


class Interface:
    def __init__(self, enabled=True):
        self.enabled = enabled
        self.data    = None

        # state
        self.tick    = 0
        self.updated = False


class Device:

    def __init__(self, address):
        self.address = address

        # init state
        self.interface = [Interface(), ] * DataID.CDDP_DATA_ID_COUNT.value
        self.handshook = False
