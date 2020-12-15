import asyncio
import time
import struct
import pprint
import functools
from enum import Enum

import matplotlib.pyplot as plt


class FrameInput:

    def __init__(self, **kwargs):
        self.x_analog = kwargs["x_analog"]
        self.y_analog = kwargs["y_analog"]
        self.x_mouse = kwargs["x_mouse"]
        self.y_mouse = kwargs["y_mouse"]

class FrameState:

    def __init__(self, **kwargs):
        self.frame_id = kwargs["frame_id"]
        self.timestamp = kwargs["timestamp"]
        self.code = kwargs["code"]

        self.frame_input = kwargs["frame_input"]
        

class FrameReply:

    def __init__(self, **kwargs):
        self.frame_id = kwargs["frame_id"]
        self.timestamp = kwargs["timestamp"]
        self.code = kwargs["code"]


class Session:
    def __init__(self):
        self.code = 0
        self.timestamp = int(time.time() * 1000000)

        self.frames_data = {}
        self.saved = False


    def close(self, save_to_file = False):

        # check if we should save
        if save_to_file and not self.saved:
            self.save_to_file()


    def save_to_file(self, name = None):
        if not name:
            name = f"{self.timestamp}.sav"

        print(f"Write {len(self.frames_data.keys())} frames to {name}")

        self.saved = True

    
    def add_frame(self, frame):
        self.frames_data[frame.frame_id] = frame
        self.saved = False



class IPCServer:

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
            
            # local variables
            connection_status = 0

            # start session
            current_session = Session()

            while connection_status != -1:

                # read data
                data = await self._read_data(reader, writer, 32)
                data = struct.unpack("IQiffii", data)

                # parse data
                # check packet id
                # parse data based on packet
                current_frame = FrameState(
                    frame_id  = data[0],
                    timestamp = data[1],
                    code      = data[2],
                    
                    frame_input = FrameInput(
                        x_analog = data[3],
                        y_analog = data[4],
                        x_mouse  = data[5],
                        y_mouse  = data[6]
                    )
                )

                # use data

                # update session
                current_session.add_frame(current_frame)

                # create reply
                current_reply = FrameReply(
                    frame_id = current_frame.frame_id,
                    timestamp = 0,
                    code = current_frame.code
                )

                # write reply

                reply_data = struct.pack(
                    "IQixxxxxxxxxxxxxxxx", 
                    current_reply.frame_id,
                    current_reply.timestamp,
                    current_reply.code
                )

                await self._write_data(reader, writer, reply_data)

            writer.close()

            # close session
            current_session.close(save_to_file = True)
        
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
    server = IPCServer("localhost", 8080, debug = True)

    # start server
    await server.start()

if __name__ == "__main__":
    asyncio.run(main())