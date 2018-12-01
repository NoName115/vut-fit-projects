NAME
====

**ftrestd** — run RESTful server

**ftrest** — run RESTful client

SYNOPSIS
========

| **ftrestd** \[**-r** ROOT-FOLDER] \[**-p** PORT]

| **ftrest** COMMAND REMOTE-PATH \[LOCAL-PATH]

DESCRIPTION
===========

**ftrestd** Server, that accept PUT, GET and DELETE commands.
And communicate via HTTP header, send JSON output and can recieve and send files.

**ftrest** Client, that accept del, get, put, lst, mkd and rmd commands.
Communicate via HTTP header, can send and recieve files and JSON output.

Options
-------

**ftrestd**
-r \[ROOT-FOLDER]

:   Set root-folder for storing users files, default is server local folder.

-p \[PORT]

:   Set port, default port is 6677.


**ftrest**
COMMAND

del:   Delete file specified by REMOTE-PATH

get:   Copy file from REMOTE-PATH to local folder or path specified by LOCAL-PATH

put:   Copy file from LOCAL-PATH to REMOTE-PATH folder

lst:   Print REMOTE-PATH folder contents to stdout (same as $ ls)

mkd:   Create directory specified by REMOTE-PATH

rmd:   Delete directory specified by REMOTE-PATH

REMOTE-PATH

: Specified REMOTE-PATH, format: http://host:port/USER-ACCOUNT/REMOTE-PATH

LOCAL-PATH

: Specified LOCAL-PATH, mandatory for command put

ERROR CODES
=====

**0**

:   Program run correctly

**1**

:   Something went wrong, error printed to stderr

AUTHOR
======

Róbert Kolcún, xkolcu00 <xkolcu00@stud.fit.vutbr.cz>
