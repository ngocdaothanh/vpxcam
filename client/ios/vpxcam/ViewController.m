#include <libavcodec/avcodec.h>

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    avcodec_register_all();

    unsigned int codec = AV_CODEC_ID_VP8;
    
    AVCodec* decoder = avcodec_find_decoder(codec);
    
    printf("%p\n", decoder);
    
    printf("%s\n", avcodec_get_name(codec));
    
    printf("%d\n", av_codec_is_encoder(decoder));
    printf("%d\n", av_codec_is_decoder(decoder));

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
