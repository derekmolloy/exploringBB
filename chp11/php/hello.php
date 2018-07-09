<?php $temperature = shell_exec('/usr/local/bin/tmp36raw'); ?>
<html><head><title>Beagle Board PHP Test</title></head>
 <body>
 <h2>Hello from the Beagle Board!</h2>
 <p>Your IP address is: <?php echo $_SERVER['REMOTE_ADDR']; ?></p>
 <p>The temperature at the board is: <?php echo $temperature ?> &#186C</p>
 </body>
</html>

