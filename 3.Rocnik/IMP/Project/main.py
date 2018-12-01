'''
Script run 2 servers, socket server for collecting data
and http server.

Author: Robert Kolcun, xkolcu00
'''
from server import run_socket_server
from http_server import run_http_server
from multiprocessing import Process

try:
    socket_process = Process(target=run_socket_server)
    http_process = Process(target=run_http_server)

    socket_process.start()
    http_process.start()
except KeyboardInterrupt:
    socket_process.terminate()
    http_process.terminate()
