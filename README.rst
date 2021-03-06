Video only, no audio:

::

  camera -> capture -> encode -> server ---> client -> decode -> display

Dependencies
------------

* opencv: Capture frames from camera, display frames, convert RGB <-> YV12
* libvpx: Encode/decode frames; libvpx only works with YV12, not RGB
* boost: Server and client

On Mac, if you use Homebrew:

::

  brew install opencv
  brew install libvpx
  brew install boost

If you use MacPorts:

::

  sudo port install opencv
  sudo port install libvpx
  sudo port install boost

Build and run
-------------

::

  make

Server
~~~~~~

::

  vpxcams <cam_id> <port>

Ex:

::

  vpxcams 0 9000

Client
~~~~~~

::

  vpxcamc <host> <port>

Ex:

::

  vpxcamc localhost 9000

Network protocol
----------------

TCP is used.

Client will receive:

::

  <width: int>
  <height: int>

  <size: int>
  <encoded frame: bytes>

  <size: int>
  <encoded frame: bytes>

  ...

References
----------

* http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture
* http://www.webmproject.org/docs/vp8-sdk/samples.html
* http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/examples.html
* http://www.fourcc.org/yuv.php
