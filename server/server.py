import asyncio
import CDDP

# import uvicorn

# from fastapi import FastAPI


async def main():
    # local variables
    cddp_log = []

    # create servers
    # web_server = FastAPI()
    cddp_server = CDDP.Server(
        "localhost",
        8989
    )

    # configure servers
    # @web_server.get("/")
    # async def root():
    #     return "Hello World"
    #
    # @web_server.get("/log")
    # async def get_cddp_log():
    #
    #     # read cddp log queue into cddp log data
    #     while not cddp_server.log_queue.empty():
    #         cddp_log.append(cddp_server.log_queue.get())
    #
    #     # return cddp log data
    #     return {
    #         "log": cddp_log
    #     }

    # start servers
    await cddp_server.start()
    # uvicorn.run(web_server, host="0.0.0.0", port=8000)

    while True:
        pass

    return


if __name__ == "__main__":
    asyncio.run(main())