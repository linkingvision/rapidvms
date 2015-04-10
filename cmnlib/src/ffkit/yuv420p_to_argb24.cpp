
#include "ffkit/yuv420p_to_argb24.h"

#include "cppkit/ck_exception.h"
#include "cppkit/ck_socket.h"

using namespace ffkit;
using namespace cppkit;
using namespace std;

#define GETR(y,u,v) ((1.164 * (y - 16)) + (1.596 * ((v) - 128)))
#define GETG(y,u,v) ((1.164 * (y - 16)) - (0.813 * ((v) - 128)) - (0.391 * ((u) - 128)))
#define GETB(y,u,v) ((1.164 * (y - 16)) + (2.018 * ((u) - 128)))

#define CLIP(val,min,max) (val<min)?min:(val>max)?max:val

yuv420p_to_argb24::yuv420p_to_argb24() :
    _rgb24(),
    _pf( std::make_shared<av_packet_factory_default>() )
{
}

yuv420p_to_argb24::~yuv420p_to_argb24() throw()
{
}

void yuv420p_to_argb24::transform( shared_ptr<av_packet> input, size_t width, size_t height )
{
    size_t dataSize = height * (width*4);
    _rgb24 = _pf->get( dataSize );
    _rgb24->set_data_size( dataSize );

    uint8_t* dst = _rgb24->map();

    const uint8_t* srcY = input->map();
    const uint8_t* srcU = srcY + (width*height);
    const uint8_t* srcV = srcU + ((width/2)*(height/2));

    for( size_t dstY = 0; dstY < height; dstY++ )
    {
        for( size_t dstX = 0; dstX < width; dstX++ )
        {
            double dstFracY = ((double)dstY / height);
            double dstFracX = ((double)dstX / width);

            size_t uvY = (size_t)(dstFracY*(height/2));
            size_t uvX = (size_t)(dstFracX*(width/2));

            uint8_t* writer = dst + (( dstY * (width*4) ) + (dstX * 4));
            const uint8_t* readerY = srcY + (( dstY * width ) + dstX );
            const uint8_t* readerU = srcU + (( uvY * (width/2) ) + uvX );
            const uint8_t* readerV = srcV + (( uvY * (width/2) ) + uvX );

            uint32_t pixel = 0;
            uint8_t* pixelWriter = (uint8_t*)&pixel;

            *pixelWriter = 0x00;
            pixelWriter++;
            *pixelWriter = (uint8_t)(CLIP(GETR( *readerY, *readerU, *readerV ),0,255));
            pixelWriter++;
            *pixelWriter = (uint8_t)(CLIP(GETG( *readerY, *readerU, *readerV ),0,255));
            pixelWriter++;
            *pixelWriter = (uint8_t)(CLIP(GETB( *readerY, *readerU, *readerV ),0,255));

            uint32_t word = ntohl( pixel );
            *((uint32_t*)writer) = word;
        }
    }
}

shared_ptr<av_packet> yuv420p_to_argb24::get()
{
    return std::move( _rgb24 );
}
