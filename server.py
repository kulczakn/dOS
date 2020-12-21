import asyncio
import time
import struct
import pprint
import functools
from enum import Enum

# import matplotlib.pyplot as plt


class CDDPServer:

    def __init__(self, host, port, debug = False):
        # parameters
        self.host = host
        self.port = port

        # initialize members
        self.server = None
        self.address = None
        self.connected = False


    # public methods

    async def start(self):
        self.server = await asyncio.start_server(
            self._get_handler(), self.host, self.port
        )

        self.address = self.server.sockets[0].getsockname()

        async with self.server:
            await self.server.serve_forever()

        self.connected = True
        

    # private methods

    def _get_handler(self):

        async def handler(reader, writer):

            print("Handler")
            
            # local variables
            connection_status = 0

            while connection_status != -1:

                print("Awaiting data...")

                # read data
                data = await self._read_data(reader, writer, 128)
                data = struct.unpack("IxxxxQ112c", data)

                # parse data
                # check packet id
                # parse data based on packet
                print(f"id:   {data[0]}")
                print(f"tick: {data[1]}")
                print(f"data: {data[2]}\n")

                # use data

                # update session

                # create reply

                # write reply

                # await self._write_data(reader, writer, reply_data)

            writer.close()

            # close session
            current_session.close(save_to_file = True)
        
        print("Handler created\n")
        return handler


    async def _read_data(self, reader, writer, bytes):

        # fetch packet from stream
        data = await reader.read(bytes)

        return data


    async def _write_data(self, reader, writer, reply):
        writer.write(reply)
        await writer.drain()


async def main():

    # initialize server
    # server = IPCServer("localhost", 8080, debug = True)
    server = CDDPServer("localhost", 8989, debug = True)

    print("Server created\n")

    # start server
    await server.start()

if __name__ == "__main__":
    asyncio.run(main())