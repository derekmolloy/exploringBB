<?php $temperature = shell_exec('/usr/local/bin/tmp36raw'); ?>

<html><head><title>BBB PHP Test</title></head>
 <body>
 Hello from the BeagleBone!
 <p>Your IP address is: <?php echo $_SERVER['REMOTE_ADDR']; ?></p>
 <p>The temperature at the BBB is: <?php echo $temperature ?> &#186C</p>
 </body>
</html>
