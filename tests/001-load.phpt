--TEST--
Check if rindow_opencl is loaded
--SKIPIF--
<?php
if (!extension_loaded('rindow_clblast')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "rindow_clblast" is available';
?>
--EXPECT--
The extension "rindow_clblast" is available
