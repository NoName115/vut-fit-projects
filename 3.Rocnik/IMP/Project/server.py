'''
Script run socket server and save recieved data to file

Author: Robert Kolcun, xkolcu00
'''
import json
import socket
import os
import csv
from datetime import datetime


PORT = 12345

def save_data(record):
    '''
    Save input record into csv file

    Arguments:
        record - dictionary, id and temperature
    '''
    print("Data: " + str(record))

    path = 'data/'
    if (not os.path.exists(path)):
        os.mkdir(path)

    csv_file = open(path + str(record['id']) + ".csv", 'a')
    csv_writer = csv.writer(
        csv_file,
        delimiter=',',
        quotechar='\"',
        quoting=csv.QUOTE_MINIMAL
    )
    now = datetime.now()
    csv_writer.writerow([
        now.strftime("%d.%m.%Y"),
        now.strftime("%H:%M:%S"),
        str(round(record['temp'], 2)),
    ])
    csv_file.close()

def run_socket_server():
    '''
    Run socket server and call sava_data method.
    '''
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(('', PORT))
        sock.listen(5)
    except Exception as err:
        print("ERROR: " + str(err))
        return

    while True:
        try:
            print("Ready for client")
            new_sock, in_addr = sock.accept()
            # Resolve data and save them
            data = json.loads(new_sock.recv(4096).decode('UTF-8'))
            save_data(data)
        except KeyboardInterrupt:
            sock.shutdown(socket.SHUT_RDWR)
            sock.close()
