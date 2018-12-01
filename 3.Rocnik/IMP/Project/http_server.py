'''
Script responsible for http server.

Author: Robert Kolcun, xkolcu00
'''
from http.server import BaseHTTPRequestHandler, HTTPServer
import os
import glob
import csv
import re


PORT_NUMBER = 8080
HOST_IP = '127.0.0.1'

def load_index():
    '''
    Load data from file and generate index.html page.
    '''
    def get_html_row(file_path, record):
        '''
        Return one row for main table fot index.html file.
        '''
        re_find = re.search(r'\/(\d+)\.csv$', file_path)
        device_id = re_find.groups()[0]

        '''
        row_string = '<tr>\n'
        row_string += '<td>' + device_id + '</td>\n'
        row_string += '\n'.join(
            '<td>' + str(item) + '</td>' for item in record
        )
        row_string += '<td><a href=/history/' + device_id + '.html>History</a></td>\n'
        row_string += '</tr>'
        '''

        row_string = '<div class="row">\n'
        row_string += '<div class="cell" data-title="ID">' + device_id + '</div>\n'
        row_string += '<div class="cell" data-title="Date">' + record[0] + '</div>\n'
        row_string += '<div class="cell" data-title="Time">' + record[1] + '</div>\n'
        row_string += '<div class="cell" data-title="Last temperature">' + record[2] + '</div>\n'
        row_string += '<div class="cell" data-title="Date"><a href=/history/' + device_id + '.html>History</a></div>\n'
        row_string += '</div>\n'

        return row_string

    index_html = open('index.html', 'r').read()

    # Generate table rows
    table_rows = []
    file_list = []
    if (os.path.exists('data/')):
        file_list = glob.glob("data/*.csv")

    for file in file_list:
        csv_file = open(file, 'r')
        csv_reader = csv.reader(
            csv_file,
            delimiter=',',
            quotechar='\"',
            quoting=csv.QUOTE_MINIMAL
        )
        for line in csv_reader:
            pass
        table_rows.append(get_html_row(
            file,
            line
        ))

    index_html = index_html.replace(
        '<!-- CONTENT -->',
        ''.join(table_rows)
    )
    return index_html

def load_history(path):
    '''
    Return html site for device history.
    '''
    content_string = ""
    csv_file = open(
        path.replace('/history', 'data').replace('html', 'csv'),
        'r'
    )
    csv_reader = csv.reader(
        csv_file,
        delimiter=',',
        quotechar='\"',
        quoting=csv.QUOTE_MINIMAL
    )
    for line in csv_reader:
        row_string = '<div class="row">\n'
        row_string += '<div class="cell" data-title="Date">' + line[0] + '</div>\n'
        row_string += '<div class="cell" data-title="Time">' + line[1] + '</div>\n'
        row_string += '<div class="cell" data-title="Temperature">' + line[2] + '</div>\n'
        row_string += '</div>\n'

        content_string = row_string + content_string

    history_html = open('history.html', 'r').read()
    history_html = history_html.replace(
        '<!-- CONTENT -->',
        content_string
    )
    history_html = history_html.replace(
        '<!-- IOT ID -->',
        re.search(r'\/(\d+)\.html$', path).groups()[0]
    )

    return history_html

class myHandler(BaseHTTPRequestHandler):
	
	def do_GET(self):
		self.send_response(200)
		self.send_header('Content-type','text/html')
		self.end_headers()

        # Send the html message
		self.wfile.write(bytes(
            load_history(self.path)
                if (self.path.find('history') != -1) else load_index(),
            'UTF8'
        ))

def run_http_server():
    try:
        server = HTTPServer((HOST_IP, PORT_NUMBER), myHandler)
        print('Started httpserver on port', PORT_NUMBER)
        server.serve_forever()
    except KeyboardInterrupt:
        print('^C received, shutting down the web server')
        server.socket.close()
