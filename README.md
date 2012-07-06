#README

Wrapper of libartnet for OF 007 on OSX 10.6 and 10.7.
May be useful for Linux and M$ too - see libartnet docs.

Can send to multiple artnet devices on the same network subnet if specifying sendRaw in the setup() function.

Receiving callback is untested but should work for a single artnet device. 

Based on code here:
http://www.opendmx.net/index.php/IPad_ArtNet_Node
Thanks to help from the OLA community


#INSTALLATION

* Copy the folder to your OF addons subfolder
* Include ofxLibArtnet in your OF project (removing the example subfolder).
* Add the libs subfolder as a recursive header search path in your project settings (e.g. $OF_PATH/addons/ofxLibArtnet/libs/**)
* Add the preprocessor macro "HAVE_CONFIG_H" to your project settings.



