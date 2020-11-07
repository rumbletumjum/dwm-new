/* See LICENSE file for copyright and license details. */

#define BROWSER "brave"
#define TERMINAL "st"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */

static const char *fonts[]               = { "xos4 Terminus:pixelsize=28:style=bold:antialias=false:autohint=false" };
static const char dmenufont[]            = "xos4 Terminus:pixelsize=28:style=bold:antialias=false:autohint=false";

static const char col_black[]       = "#000000";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan1[]       = "#005577";
static const char col_cyan2[]       = "#285577";
static const char col_bblue[]       = "#6699cc";
static const char col_blue2[]       = "#1177AA";
static const char col_blue3[]       = "#006699";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
    [SchemeNorm]   = { col_gray3, col_gray1, col_gray2 },
    [SchemeStatus] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]    = { col_gray4, col_blue2, col_bblue },
	[SchemeBar]    = { col_gray4, col_blue2, col_bblue },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

const char *spcmd1[] = {"st", "-n", "spterm", "-g", "80x24", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "lf", NULL };
const char *spcmd3[] = {"st", "-n", "spspotify", "-g", "132x43", "-e", "spt", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"spspotify",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance       title       tags mask     isfloating   monitor */
	{ "Gimp",           NULL,          NULL,       0,            1,           -1 },
	{ "Firefox",        NULL,          NULL,       1 << 8,       0,           -1 },
	{ "Brave-browser",  NULL,          NULL,       1 << 1,       0,           -1 },
	{ "Thunar",         NULL,          NULL,       0,            1,           -1 },
	{ "Lxappearance",   NULL,          NULL,       0,            1,           -1 },
	{ "File-roller",    NULL,          NULL,       0,            1,           -1 },
	{ "qBittorrent",    NULL,          NULL,       1 << 8,       0,           -1 },
	{ "Surf",           NULL,          NULL,       1 << 3,       0,           -1 },
	{ NULL,             "spterm",      NULL,       SPTAG(0),     1,           -1 },
	{ NULL,             "spfm",        NULL,       SPTAG(1),     1,           -1 },
	{ NULL,             "spspotify",   NULL,       SPTAG(2),     1,           -1 },
};

/* layout(s) */
static const float mfact         = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster         = 1;    /* number of clients in master area */
static const int resizehints     = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 0;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },

	{ "[@]",      spiral },
	{ "[\\]",     dwindle },

	{ ":::",      gaplessgrid },
	{ "###",      nrowgrid },

	{ "[M]",      monocle }, 
	{ "H[]",      deck },

	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },

	{ "><>",      NULL },    /* no layout function means floating behavior */
	/*{ "TTT",      bstack },
	{ "[\\]",     dwindle },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ ">M>",      centeredfloatingmaster }, */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define MOD1KEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MOD1KEY,                      KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MOD1KEY|ShiftMask,            KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static       char dmenumon[2]      = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_bblue, "-sf", col_black, "-h", "30", NULL };
static const char *termcmd[]       = { "st", "-e", "fish", NULL };
static const char *windowmenucmd[] = { "windowmenu", NULL };
static const char *browsercmd[]    = { "brave", NULL };


static Key keys[] = {
	/* modifier                       key               function        argument */
	{ MODKEY,                         XK_p,             spawn,          {.v = dmenucmd }      },
	{ MODKEY,                         XK_d,             spawn,          {.v = windowmenucmd } },
	{ MODKEY|ShiftMask,               XK_Return,        spawn,          {.v = termcmd }       },
	{ MODKEY,                         XK_b,             spawn,          {.v = browsercmd }    },
	{ MODKEY,                         XK_comma,         incnmaster,     {.i = +1 }            },
	{ MODKEY,                         XK_period,        incnmaster,     {.i = -1 }            },
	{ MOD1KEY|ShiftMask,              XK_j,             inplacerotate,  {.i = +1}             },
	{ MOD1KEY|ShiftMask,              XK_k,             inplacerotate,  {.i = -1}             },
	{ MOD1KEY|ShiftMask,              XK_h,             inplacerotate,  {.i = +2}             },
	{ MOD1KEY|ShiftMask,              XK_l,             inplacerotate,  {.i = -2}             },
	{ MODKEY,                         XK_h,             setmfact,       {.f = -0.05}          },
	{ MODKEY,                         XK_l,             setmfact,       {.f = +0.05}          },
	{ MODKEY,                         XK_r,             reorganizetags, {0}                   },
	{ MODKEY,                         XK_Return,        zoom,           {0}                   },
	{ MODKEY,                         XK_m,             focusmaster,    {0}                   },
	{ MODKEY,                         XK_Tab,           view,           {0}                   },
	{ MODKEY,                         XK_w,             killclient,     {0}                   },

	{ MODKEY,                         XK_t,             setlayout,      {.v = &layouts[0]}    },
	{ MODKEY|ShiftMask,               XK_t,             setlayout,      {.v = &layouts[1]}    }, 

	{ MODKEY,                         XK_y,             setlayout,      {.v = &layouts[2]}    },
	{ MODKEY|ShiftMask,               XK_y,             setlayout,      {.v = &layouts[3]}    },

	{ MODKEY,                         XK_u,             setlayout,      {.v = &layouts[4]}    },
	{ MODKEY|ShiftMask,               XK_u,             setlayout,      {.v = &layouts[5]}    },

	{ MODKEY,                         XK_i,             setlayout,      {.v = &layouts[6]}    },
	{ MODKEY|ShiftMask,               XK_i,             setlayout,      {.v = &layouts[7]}    },

	{ MODKEY,                         XK_o,             setlayout,      {.v = &layouts[8]}    },
	{ MODKEY|ShiftMask,               XK_o,             setlayout,      {.v = &layouts[9]}    },

	{ MODKEY,                         XK_space,         setlayout,      {.v = &layouts[10]}   },
	{ MODKEY|ShiftMask,               XK_space,         togglefloating, {0}                   },

	{ MODKEY,                         XK_0,             view,           {.ui = ~0 }           },
	{ MODKEY|ShiftMask,               XK_0,             tag,            {.ui = ~0 }           },
	{ MODKEY|ShiftMask,               XK_comma,         tagmon,         {.i = -1 }            },
	{ MODKEY|ShiftMask,               XK_period,        tagmon,         {.i = +1 }            },
	{ MODKEY,                         XK_g,             togglescratch,  {.ui = 0 }            },
	{ MODKEY,                         XK_f,             togglescratch,  {.ui = 1 }            },
	{ MODKEY,                         XK_e,             togglescratch,  {.ui = 2 }            },
	{ Mod1Mask|ShiftMask,             XK_BackSpace,     quit,           {0}                   },
	{ Mod1Mask|Mod4Mask,              XK_u,             incrgaps,       {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_u,             incrgaps,       {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_i,             incrigaps,      {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_i,             incrigaps,      {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_o,             incrogaps,      {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_o,             incrogaps,      {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_6,             incrihgaps,     {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_6,             incrihgaps,     {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_7,             incrivgaps,     {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_7,             incrivgaps,     {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_8,             incrohgaps,     {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_8,             incrohgaps,     {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_9,             incrovgaps,     {.i = +1 }            },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_9,             incrovgaps,     {.i = -1 }            },
	{ Mod1Mask|Mod4Mask,              XK_0,             togglegaps,     {0}                   },
	{ Mod1Mask|Mod4Mask|ShiftMask,    XK_0,             defaultgaps,    {0}                   },
	
	STACKKEYS(MODKEY,                                   focus)
	STACKKEYS(MODKEY|ShiftMask,                         push )
	TAGKEYS(                          XK_1,                 0)
	TAGKEYS(                          XK_2,                 1)
	TAGKEYS(                          XK_3,                 2)
	TAGKEYS(                          XK_4,                 3)
	TAGKEYS(                          XK_5,                 4)
	TAGKEYS(                          XK_6,                 5)
	TAGKEYS(                          XK_7,                 6)
	TAGKEYS(                          XK_8,                 7)
	TAGKEYS(                          XK_9,                 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0}                },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0}                },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd }    },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0}                },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0}                },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0}                },
	{ ClkTagBar,            0,              Button1,        view,           {0}                },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0}                },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0}                },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0}                },
};
