#include <libavcodec/avcodec.h>

#import "ViewController.h"
#import "GCDAsyncSocket.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    avcodec_register_all();
    AVCodec* decoder = avcodec_find_decoder(AV_CODEC_ID_VP8);

    GCDAsyncSocket* socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
