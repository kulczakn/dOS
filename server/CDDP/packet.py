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
