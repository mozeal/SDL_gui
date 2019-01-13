//
//  GUI_IconView.hpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_IconView_hpp
#define GUI_IconView_hpp

#include <stdio.h>
#include "GUI_TextView.h"

class GUI_IconView : public GUI_TextView {
public:
    static GUI_IconView *create( GUI_View *parent, uint16_t unicode, const char *fontname, int fontsize, int x=0, int y=0, int width=0, int height=0,
                                std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    GUI_IconView(GUI_View *parent, uint16_t unicode, const char *fontname, int fontsize, int x=0, int y=0, int width=0, int height=0,
                 std::function<bool(SDL_Event* ev)>userEventHandler = NULL);
    virtual ~GUI_IconView();
    
    SDL_Texture* createTextureFormUnicode(Uint16 unicode, SDL_Rect* rect=NULL);
};

#define kIcon_address_book              0xf2b9u
#define kIcon_address_card              0xf2bbu
#define kIcon_angry                     0xf556u
#define kIcon_arrow_alt_circle_down     0xf358u
#define kIcon_arrow_alt_circle_left     0xf359u
#define kIcon_arrow_alt_circle_right    0xf35au
#define kIcon_arrow_alt_circle_up       0xf35bu
#define kIcon_bell                      0xf0f3u
#define kIcon_bell_slash                0xf1f6u
#define kIcon_bookmark                  0xf02eu
#define kIcon_building                  0xf1adu
#define kIcon_calendar                  0xf133u
#define kIcon_calendar_alt              0xf073u
#define kIcon_calendar_check            0xf274u
#define kIcon_calendar_minus            0xf272u
#define kIcon_calendar_plus             0xf271u
#define kIcon_calendar_times            0xf273u
#define kIcon_caret_square_down         0xf150u
#define kIcon_caret_square_left         0xf191u
#define kIcon_caret_square_right        0xf152u
#define kIcon_caret_square_up           0xf151u
#define kIcon_chart_bar                 0xf080u
#define kIcon_check_circle              0xf058u
#define kIcon_check_square              0xf14au
#define kIcon_circle                    0xf111u
#define kIcon_clipboard                 0xf328u
#define kIcon_clock                     0xf017u
#define kIcon_clone                     0xf24du
#define kIcon_closed_captioning         0xf20au
#define kIcon_comment                   0xf075u
#define kIcon_comment_alt               0xf27au
#define kIcon_comment_dots              0xf4adu
#define kIcon_comments                  0xf086u
#define kIcon_compass                   0xf14eu
#define kIcon_copy                      0xf0c5u
#define kIcon_copyright                 0xf1f9u
#define kIcon_credit_card               0xf09du
#define kIcon_dizzy                     0xf567u
#define kIcon_dot_circle                0xf192u
#define kIcon_edit                      0xf044u
#define kIcon_envelope                  0xf0e0u
#define kIcon_envelope_open             0xf2b6u
#define kIcon_eye                       0xf06eu
#define kIcon_eye_slash                 0xf070u
#define kIcon_file                      0xf15bu
#define kIcon_file_alt                  0xf15cu
#define kIcon_file_archive              0xf1c6u
#define kIcon_file_audio                0xf1c7u
#define kIcon_file_code                 0xf1c9u
#define kIcon_file_excel                0xf1c3u
#define kIcon_file_image                0xf1c5u
#define kIcon_file_pdf                  0xf1c1u
#define kIcon_file_powerpoint           0xf1c4u
#define kIcon_file_video                0xf1c8u
#define kIcon_file_word                 0xf1c2u
#define kIcon_flag                      0xf024u
#define kIcon_flushed                   0xf579u
#define kIcon_folder                    0xf07bu
#define kIcon_folder_open               0xf07cu
#define kIcon_font_awesome_logo_full    0xf4e6u
#define kIcon_frown                     0xf119u
#define kIcon_frown_open                0xf57au
#define kIcon_futbol                    0xf1e3u
#define kIcon_gem                       0xf3a5u
#define kIcon_grimace                   0xf57fu
#define kIcon_grin                      0xf580u
#define kIcon_grin_alt                  0xf581u
#define kIcon_grin_beam                 0xf582u
#define kIcon_grin_beam_sweat           0xf583u
#define kIcon_grin_hearts               0xf584u
#define kIcon_grin_squint               0xf585u
#define kIcon_grin_squint_tears         0xf586u
#define kIcon_grin_stars                0xf587u
#define kIcon_grin_tears                0xf588u
#define kIcon_grin_tongue               0xf589u
#define kIcon_grin_tongue_squint        0xf58au
#define kIcon_grin_tongue_wink          0xf58bu
#define kIcon_grin_wink                 0xf58cu
#define kIcon_hand_lizard               0xf258u
#define kIcon_hand_paper                0xf256u
#define kIcon_hand_peace                0xf25bu
#define kIcon_hand_point_down           0xf0a7u
#define kIcon_hand_point_left           0xf0a5u
#define kIcon_hand_point_right          0xf0a4u
#define kIcon_hand_point_up             0xf0a6u
#define kIcon_hand_pointer              0xf25au
#define kIcon_hand_rock                 0xf255u
#define kIcon_hand_scissors             0xf257u
#define kIcon_hand_spock                0xf259u
#define kIcon_handshake                 0xf2b5u
#define kIcon_hdd                       0xf0a0u
#define kIcon_heart                     0xf004u
#define kIcon_hospital                  0xf0f8u
#define kIcon_hourglass                 0xf254u
#define kIcon_id_badge                  0xf2c1u
#define kIcon_id_card                   0xf2c2u
#define kIcon_image                     0xf03eu
#define kIcon_images                    0xf302u
#define kIcon_keyboard                  0xf11cu
#define kIcon_kiss                      0xf596u
#define kIcon_kiss_beam                 0xf597u
#define kIcon_kiss_wink_heart           0xf598u
#define kIcon_laugh                     0xf599u
#define kIcon_laugh_beam                0xf59au
#define kIcon_laugh_squint              0xf59bu
#define kIcon_laugh_wink                0xf59cu
#define kIcon_lemon                     0xf094u
#define kIcon_life_ring                 0xf1cdu
#define kIcon_lightbulb                 0xf0ebu
#define kIcon_list_alt                  0xf022u
#define kIcon_map                       0xf279u
#define kIcon_meh                       0xf11au
#define kIcon_meh_blank                 0xf5a4u
#define kIcon_meh_rolling_eyes          0xf5a5u
#define kIcon_minus_square              0xf146u
#define kIcon_money_bill_alt            0xf3d1u
#define kIcon_moon                      0xf186u
#define kIcon_newspaper                 0xf1eau
#define kIcon_object_group              0xf247u
#define kIcon_object_ungroup            0xf248u
#define kIcon_paper_plane               0xf1d8u
#define kIcon_pause_circle              0xf28bu
#define kIcon_play_circle               0xf144u
#define kIcon_plus_square               0xf0feu
#define kIcon_question_circle           0xf059u
#define kIcon_registered                0xf25du
#define kIcon_sad_cry                   0xf5b3u
#define kIcon_sad_tear                  0xf5b4u
#define kIcon_save                      0xf0c7u
#define kIcon_share_square              0xf14du
#define kIcon_smile                     0xf118u
#define kIcon_smile_beam                0xf5b8u
#define kIcon_smile_wink                0xf4dau
#define kIcon_snowflake                 0xf2dcu
#define kIcon_square                    0xf0c8u
#define kIcon_star                      0xf005u
#define kIcon_star_half                 0xf089u
#define kIcon_sticky_note               0xf249u
#define kIcon_stop_circle               0xf28du
#define kIcon_sun                       0xf185u
#define kIcon_surprise                  0xf5c2u
#define kIcon_thumbs_down               0xf165u
#define kIcon_thumbs_up                 0xf164u
#define kIcon_times_circle              0xf057u
#define kIcon_tired                     0xf5c8u
#define kIcon_trash_alt                 0xf2edu
#define kIcon_user                      0xf007u
#define kIcon_user_circle               0xf2bdu
#define kIcon_window_close              0xf410u
#define kIcon_window_maximize           0xf2d0u
#define kIcon_window_minimize           0xf2d1u
#define kIcon_window_restore            0xf2d2u

#endif /* GUI_IconView_hpp */
