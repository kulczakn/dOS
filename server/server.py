import asyncio
import CDDP

import uvicorn
from fastapi import FastAPI


async def main():
    # local variables

    # create CDDP server
    cddp_server = CDDP.Server(
        "localhost",
        8989
    )

    # run CDDP Server
    await cddp_server.run()


if __name__ == "__main__":
    asyncio.run(main())
