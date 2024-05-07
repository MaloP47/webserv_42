#!/usr/bin/env python

# Import CGI module
import cgi

# Required header for CGI scripts
print("Content-Type: text/html\n")

# Get form data
form = cgi.FieldStorage()

# Get the value of the 'name' parameter
name = form.getvalue('name')

# Check if 'name' parameter is present
if name:
    print("<h1>Hello, {}!</h1>".format(name))
else:
    print("<h1>Hello, World!</h1>")
