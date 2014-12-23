#!/bin/bash
echo "Content-type: text/html"
echo ""
echo '<html>'
echo '<head>'
echo '<meta http-equiv="Content-Type" content="text/html"; charset=UTF-8">'
echo '<title>Hello BeagleBone</title></head>'
echo '<body><h1>Hello BeagleBone</h1><para>'
echo 'The temperature in the room is '
/usr/local/bin/tmp36raw
echo ' degress Celsius '
echo '</para></html>'
