import CDDP
import ctypes


def main():
    interface_data = CDDP.IntrfData()
    interface_data.id = 23
    interface_data.frmt = 7
    interface_data.wrtbl = True
    interface_data.count = 3
    interface_data.size = 7
    interface_data.name = bytes("Test", "utf8")

    interface_pkt = CDDP.Packet()
    interface_pkt.addr = 115
    interface_pkt.id = 116
    interface_pkt.tick = 117
    interface_pkt.seq = 118
    interface_pkt.data = (ctypes.c_uint8*64)(*list(bytearray(interface_data)))  # please find a better way... maybe switch data buffers to char types?

    print(len(list(bytearray(interface_pkt))))


if __name__ == "__main__":
    main()
