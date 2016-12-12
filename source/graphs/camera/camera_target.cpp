#include "camera_target.h"

camera_target::camera_target()
{
    this->name = "CameraTarget";
}

void camera_target::event(const bs_event &e)
{
    if (e.button () & Event_MouseRight)
    {
        if (e.button () & Event_StateDown)
        {
            mouse = scale_mouse( vec2(e.x (), e.y ()) );
            panning = Camera_Target;
            if (e.modifier () & Event_ModifierAlt)
                reset ();
        }
        else if (e.button () & Event_StateUp)
            panning = Camera_Not;
    }

    if (e.button () & Event_StateMove)
    {
        if (panning == Camera_Target)
        {
            vec2 newMouse = scale_mouse( vec2(e.x (), e.y ()) );

            if (e.modifier () & Event_ModifierControl)
                move(newMouse);
            else if (e.modifier () & Event_ModifierShift)
                zoom(newMouse);
            else if (e.modifier () & Event_ModifierAlt)
            {
                //reset ();
            }
            else
                rotation (newMouse);

             mouse = newMouse;
        }
    }

    if (e.button () & Event_MouseWheel && e.button () & Event_StateDown)
    {
        if (e.modifier () & Event_ModifierShift)
            roll(Roll_Horizontal, roll_angle(.5f, e.wheel () > 0 ? true : false));
        else
            zoom( zoom_factor( 10.f, e.wheel () > 0 ? true : false) );
    }

    if (e.key () & Event_StateDown)
    {
        if (!(e.modifier () & Event_ModifierAlt) )
        {
            panning = Camera_Target;
            if (e.modifier () & Event_ModifierControl)
            {
                switch (e.key () & Keybord_MaskField)
                {
                case Event_KeyLeft:
                    move( vec3(-1.f, 0.f, 0.f), move_length(100.f, radiuss));
                    break;
                case Event_KeyRight:
                    move( vec3(1.f, 0.f, 0.f), move_length(100.f, radiuss));
                    break;
                case Event_KeyUp:
                    move(vec3(0.f, 1.f, 0.f), move_length(100.f, radiuss));
                    break;
                case Event_KeyDown:
                    move(vec3(0.f, -1.f, 0.f), move_length(100.f, radiuss));
                    break;
                default:
                    break;
                }
            }
            else if (e.modifier () & Event_ModifierShift)
            {
                switch (e.key () & Keybord_MaskField)
                {
                case Event_KeyLeft:
                    roll(Roll_Horizontal, roll_angle(10.f, true));
                    break;
                case Event_KeyRight:
                    roll(Roll_Horizontal, roll_angle(10.f, false));
                    break;
                case Event_KeyUp:
                    zoom(zoom_factor(10.f, true));
                    break;
                case Event_KeyDown:
                    zoom(zoom_factor(10.f, false));
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (e.key () & Keybord_MaskField)
                {
                case Event_KeyLeft:
                    rotation(vec3(0.f, 1.f, 0.f),  rotation_angle(10.f));
                    break;
                case Event_KeyRight:
                    rotation(vec3(0.f, 1.f, 0.f),  -rotation_angle(10.f));
                    break;
                case Event_KeyUp:
                    rotation(vec3(1.f, 0.f, 0.f),  rotation_angle(10.f));
                    break;
                case Event_KeyDown:
                    rotation(vec3(1.f, 0.f, 0.f),  -rotation_angle(10.f));
                    break;
                default:
                    break;
                }
            }
        }
    }
    else if (e.key () & Event_StateUp)
        panning = Camera_Not;
}

void camera_target::zoom_absolute(float focallength)
{
    cameras->position( cameras->focuss - focallength * cameras->look());
}

float camera_target::center_distance() {
    return dot( focus - cameras->positions, cameras->look() );
}
