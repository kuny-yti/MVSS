#include "x11_monitor.h"

namespace impl {
struct monitor
{
    bs_vector<tMonitorInofs> sys_mi;
    x11_extension       *ext;

    void setGammaRamp(const tGammaRamps &gr, RRCrtc crtc, bool rv) // ext->vidmode.available
    {
        if (rv)
        {
            XRRCrtcGamma* gamma = XRRAllocGamma(gr.red.size());

            bs_memory::copy_sse(gamma->red, gr.red.data(), gr.red.bytecount());
            bs_memory::copy_sse(gamma->green, gr.green.data(), gr.green.bytecount());
            bs_memory::copy_sse(gamma->blue, gr.blue.data(), gr.blue.bytecount());

            XRRSetCrtcGamma(ext->display(), crtc, gamma);
            XRRFreeGamma(gamma);
        }
    #ifdef hasX11ExtXf86vmode
        else if (rv)
        {
            XF86VidModeSetGammaRamp(ext->display(),ext->screen(), gr.red.size(),
                                    gr.red.data(), gr.green.data(), gr.blue.data());
        }
    #endif
    }

    tGammaRamps getGammaRamp(RRCrtc crtc, bool rv)
    {
        tGammaRamps gr;
        if (rv)
        {
            const size_t size = XRRGetCrtcGammaSize(ext->display(), crtc);
            XRRCrtcGamma* gamma = XRRGetCrtcGamma(ext->display(), crtc);

            gr.red.resize(size);
            gr.green.resize(size);
            gr.blue.resize(size);
            bs_memory::copy_sse(gr.red.data(), gamma->red, gr.red.bytecount());
            bs_memory::copy_sse(gr.green.data(), gamma->green, gr.green.bytecount());
            bs_memory::copy_sse(gr.blue.data(), gamma->blue, gr.blue.bytecount());

            XRRFreeGamma(gamma);
        }
    #ifdef hasX11ExtXf86vmode
        else if (rv)
        {
            int size;

            XF86VidModeGetGammaRampSize(ext->display(),ext->screen(), &size);

            XF86VidModeGetGammaRamp(ext->display(),ext->screen(), size, gr.red.data(), gr.green.data(), gr.blue.data());
        }
    #endif
        return gr;
    }
    ivec2 getMonitorPos(RRCrtc crtc, bool rv) //ext->randr.available && !ext->randr.monitorBroken
    {
        ivec2 pos;
        if (rv)
        {
            XRRScreenResources* sr = XRRGetScreenResourcesCurrent(ext->display(),ext->root());
            XRRCrtcInfo* ci = XRRGetCrtcInfo(ext->display(), sr, crtc);

            pos.x = ci->x;
            pos.y = ci->y;

            XRRFreeCrtcInfo(ci);
            XRRFreeScreenResources(sr);
        }
        return pos;
    }

    static bool modeIsGood(const XRRModeInfo* mi)
    {
        return (mi->modeFlags & RR_Interlace) == 0;
    }
    static const XRRModeInfo* getModeInfo(const XRRScreenResources* sr, RRMode id)
    {
        for (int i = 0;  i < sr->nmode;  i++)
        {
            if (sr->modes[i].id == id)
                return sr->modes + i;
        }

        return NULL;
    }
    static ivec2 getVideoSize(const XRRModeInfo* mi, const XRRCrtcInfo* ci)
    {
        if (ci->rotation == RR_Rotate_90 || ci->rotation == RR_Rotate_270)
            return ivec2(mi->height, mi->width);

        return ivec2(mi->width, mi->height);
    }
    static int getVideoRefreshRate(const XRRModeInfo* mi)
    {
        if (mi->hTotal && mi->vTotal)
            return (int) ((real) mi->dotClock / ((real) mi->hTotal * (real) mi->vTotal));

        return 0;
    }
    static int getSplitBPP(int bpp)
    {
        ivec3 out;

        if (bpp == 32)
            bpp = 24;

        // Convert "bits per pixel" to red, green & blue sizes
        out = ivec3(bpp / 3, bpp / 3, bpp / 3);
        if ((bpp - (out.r * 3)) >= 1)
            out.g = out.g + 1;

        if ((bpp - (out.r * 3)) == 2)
            out.r = out.r + 1;
        return int(out.r << 24 | out.g << 16 | out.b << 8);
    }
};
}


x11_monitor::x11_monitor(x11_extension *ext)
{
    impl = bsNew(impl::monitor);
    impl->ext = ext;
}
x11_monitor::~x11_monitor()
{
    bsDelete (impl);
}
void x11_monitor::enums()
{
    if (impl->ext->randr.available)
    {
        int screenCount = 0;
        XineramaScreenInfo* screens = NULL;
        XRRScreenResources* sr = XRRGetScreenResources(impl->ext->display(),impl->ext->root());
        RROutput primary = XRRGetOutputPrimary(impl->ext->display(),impl->ext->root());

        if (impl->ext->inerama.available)
            screens = XineramaQueryScreens(impl->ext->display(), &screenCount);

        for (int i = 0;  i < sr->ncrtc;  i++)
        {
            XRRCrtcInfo* ci = XRRGetCrtcInfo(impl->ext->display(), sr, sr->crtcs[i]);

            for (int j = 0;  j < ci->noutput;  j++)
            {
                tMonitorInofs monitor;
                XRROutputInfo* oi = XRRGetOutputInfo(impl->ext->display(), sr, ci->outputs[j]);
                if (oi->connection != RR_Connected)
                {
                    XRRFreeOutputInfo(oi);
                    continue;
                }

                if (ci->rotation == RR_Rotate_90 || ci->rotation == RR_Rotate_270)
                    monitor.millimeters = vec2 (oi->mm_height, oi->mm_width);
                else
                    monitor.millimeters = vec2 (oi->mm_width, oi->mm_height);
                monitor.inches = monitor.millimeters * vec2(25.4);

                monitor.name = oi->name;
                monitor.dev.adapter = (winid)oi->crtc;
                monitor.id = (winid)ci->outputs[j];
                monitor.gamma = impl->getGammaRamp(oi->crtc, !impl->ext->randr.monitorBroken
                                   #ifdef hasX11ExtXf86vmode
                                                   && impl->ext->vidmode.available
                                   #endif
                                                   );
                monitor.pos = impl->getMonitorPos(oi->crtc, !impl->ext->randr.monitorBroken);

                for (i = 0;  i < oi->nmode;  i++)
                {
                    tMonitorColors mci;
                    const XRRModeInfo* mi = impl->getModeInfo(sr, oi->modes[i]);
                    if (!impl->modeIsGood(mi))
                        continue;

                    mci.size = impl->getVideoSize(mi, ci);
                    mci.fps = impl->getVideoRefreshRate(mi);
                    mci.bpp = impl->ext->depth();
                    mci.bits = impl->getSplitBPP(impl->ext->depth());
                    mci.dpi = (25.4 * mci.size.height) / oi->mm_height;

                    if (monitor.color.find(mci) != -1)
                        monitor.color.append(mci);
                }

                for (int k = 0;  k < screenCount;  k++)
                {
                    if (screens[k].x_org == ci->x && screens[k].y_org == ci->y &&
                        screens[k].width == ci->width && screens[k].height == ci->height)
                    {
                        monitor.index = k;
                        break;
                    }
                }

                XRRFreeOutputInfo(oi);

                if (ci->outputs[j] == primary)
                    impl->sys_mi.prepend(monitor);
                else
                    impl->sys_mi.append(monitor);
            }

            XRRFreeCrtcInfo(ci);
        }

        XRRFreeScreenResources(sr);

        if (screens)
            XFree(screens);

        if (impl->sys_mi.size() == 0)
        {
            printf("X11: RandR monitor support seems broken");

           impl->ext->randr.monitorBroken = true;
           return ;
        }
    }
}
bool x11_monitor::change(const tMonitorInofs &mi, bool is_fullscreen)
{

}
