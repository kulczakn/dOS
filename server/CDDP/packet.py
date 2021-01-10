import ctypes
import enum


CDDP_DATA_SIZE       = 64
CDDP_PKT_SIZE        = 128


class DataID(enum.Enum):
    CDDP_DATA_ID_FIRST = 0

    # System data IDs are reserved IDs used to handle module configuration and connecting to the hub
    CDDP_SYS_DATA_ID_FIRST = CDDP_DATA_ID_FIRST

    CDDP_SYS_DATA_CONN     = CDDP_SYS_DATA_ID_FIRST
    CDDP_SYS_DATA_CONNACK  = 1
    CDDP_SYS_DATA_INTRF    = 2
    CDDP_SYS_DATA_KICK     = 3
    CDDP_SYS_DATA_DISCONN  = 4

    CDDP_SYS_DATA_ID_LAST = 9

    CDDP_SNSR_ID_LAST = 19

    CDDP_RES_ID_FIRST = 100                     # 0-9 are reserved for sys IDs, which the simulator also uses
    CDDP_RES_ID_LAST  = 109

    # Simulated Data IDs are used to simulate embedded functions, they are the embedded ID counterpart offset by 100
    CDDP_SIM_DATA_ID_FIRST = 110
    CDDP_SIM_DATA_ID_LAST  = 200

    CDDP_DATA_ID_LAST = CDDP_SIM_DATA_ID_LAST

    CDDP_SYS_DATA_ID_COUNT = 10
    CDDP_SIM_DATA_ID_COUNT = 91
    CDDP_DATA_ID_COUNT     = 200


# types

class Packet(ctypes.Structure):
    _fields_ = [
        ("addr", ctypes.c_uint32),
        ("id",   ctypes.c_uint8),
        ("tick", ctypes.c_uint64),
        ("seq",  ctypes.c_uint32),
        ("data", ctypes.c_uint8 * CDDP_DATA_SIZE),

        ("buf",  ctypes.c_uint8 * (CDDP_PKT_SIZE - 1 - 2*4 - 8 - CDDP_DATA_SIZE - 3)) # extra padding for alignement
    ]


class ConnData(ctypes.Structure):
    _fields_ = [
        ("device",      ctypes.c_uint8),
        ("intrf_count", ctypes.c_uint8),

        ("buf", ctypes.c_uint8 * (CDDP_PKT_SIZE - 2*1))
    ]


class ConnackData(ctypes.Structure):
    _fields_ = [
        ("addr",       ctypes.c_uint32),
        ("intrf_cont", ctypes.c_uint8),

        ("buf", ctypes.c_uint8 * (CDDP_PKT_SIZE - 2*1))
    ]


class IntrfData(ctypes.Structure):
    _fields_ = [
        ("id", ctypes.c_uint8),
        ("frmt", ctypes.c_uint8),
        ("wrtbl", ctypes.c_bool),
        ("count", ctypes.c_uint32),
        ("size", ctypes.c_uint32),
        ("name", ctypes.c_char * 16),

        ("pad", ctypes.c_uint8 * (CDDP_DATA_SIZE - 3*1 - 2*4 - 16*1 - 1)) # extra padding for alignement
    ]
