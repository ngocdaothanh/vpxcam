::

  camera -> capture -> encode -> server ---> client -> decode -> display

Dependencies
------------

* opencv: Capture frames from camera, display frames, convert RGB <-> YV12
* libvpx: Encode/decode frames; libvpx only works with YV12, not RGB
* boost: Server and client

On Mac:

::

  brew install opencv
  brew install libvpx
  brew install boost

Build and run
-------------

::

  make

References
----------

http://www.webmproject.org/docs/vp8-sdk/samples.html
