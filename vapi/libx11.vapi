/*
 * libx11.vapi - "Strict" Vala bindings for Xlib (libx11).  These bindings
 * do not attempt to do much to the X11 interface; they are intended to be
 * used by a wrapper class library.
 *
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL v3 as published by the Free Software Foundation
 */

[CCode(cprefix = "", lower_case_cprefix = "", cheader_filename = "X11/Xlib.h")]
namespace Native.XLib {
	public const int X_PROTOCOL;
	public const int X_PROTOCOL_REVISION;

	public const long None;
	public const long ParentRelative;
	public const long CopyFromParent;
	public const long PointerWindow;
	public const long InputFocus;
	public const long PointerRoot;
	public const long AnyPropertyType;
	public const long AnyKey;
	public const long AnyButton;
	public const long AllTemporary;
	public const long CurrentTime;
	public const long NoSymbol;

	public const long NoEventMask;
	public const long KeyPressMask;
	public const long KeyReleaseMask;
	public const long ButtonPressMask;
	public const long ButtonReleaseMask;
	public const long EnterWindowMask;
	public const long LeaveWindowMask;
	public const long PointerMotionMask;
	public const long PointerMotionHintMask;
	public const long Button1MotionMask;
	public const long Button2MotionMask;
	public const long Button3MotionMask;
	public const long Button4MotionMask;
	public const long Button5MotionMask;
	public const long ButtonMotionMask;
	public const long KeymapStateMask;
	public const long ExposureMask;
	public const long VisibilityChangeMask;
	public const long StructureNotifyMask;
	public const long ResizeRedirectMask;
	public const long SubstructureNotifyMask;
	public const long FocusChangeMask;
	public const long PropertyChangeMask;
	public const long ColormapChangeMask;
	public const long OwnerGrabButtonMask;

	public const int KeyPress;
	public const int KeyRelease;
	public const int ButtonPress;
	public const int ButtonRelease;
	public const int MotionNotify;
	public const int EnterNotify;
	public const int LeaveNotify;
	public const int FocusIn;
	public const int FocusOut;
	public const int KeymapNotify;
	public const int Expose;
	public const int GraphicsExpose;
	public const int NoExpose;
	public const int VisibilityNotify;
	public const int CreateNotify;
	public const int DestroyNotify;
	public const int UnmapNotify;
	public const int MapNotify;
	public const int MapRequest;
	public const int ReparentNotify;
	public const int ConfigureNotify;
	public const int ConfigureRequest;
	public const int PropertyNotify;
	public const int SelectionClear;
	public const int SelectionRequest;
	public const int SelectionNotify;
	public const int ColormapNotify;
	public const int ClientMessage;
	public const int MappingNotify;
	public const int LASTEvent;

	public const int ShiftMask;
	public const int LockMask;
	public const int ControlMask;
	public const int Mod1Mask;
	public const int Mod2Mask;
	public const int Mod3Mask;
	public const int Mod4Mask;
	public const int Mod5Mask;

	public const int ShiftMapIndex;
	public const int LockMapIndex;
	public const int ControlMapIndex;
	public const int Mod1MapIndex;
	public const int Mod2MapIndex;
	public const int Mod3MapIndex;
	public const int Mod4MapIndex;
	public const int Mod5MapIndex;

	public const int Button1Mask;
	public const int Button2Mask;
	public const int Button3Mask;
	public const int Button4Mask;
	public const int Button5Mask;

	public const int AnyModifier;

	public const int Button1;
	public const int Button2;
	public const int Button3;
	public const int Button4;
	public const int Button5;

	public const int NotifyNormal;
	public const int NotifyGrab;
	public const int NotifyUngrab;
	public const int NotifyWhileGrabbed;

	public const int NotifyHint;

	public const int NotifyAncestor;
	public const int NotifyVirtual;
	public const int NotifyInferior;
	public const int NotifyNonlinear;
	public const int NotifyNonlinearVirtual;
	public const int NotifyPointer;
	public const int NotifyPointerRoot;
	public const int NotifyDetailNone

	public const int VisibilityUnobscured;
	public const int VisibilityPartiallyObscured;
	public const int VisibilityFullyObscured;

	public const int PlaceOnTop;
	public const int PlaceOnBottom;

	public const int FamilyInternet;
	public const int FamilyDECNet;
	public const int FamilyChaos;
	public const int FamilyInternet6;

	public const int FamilyServerInterpreted;

	public const int PropertyNewValue;
	public const int PropertyDelete;

	public const int ColormapUninstalled;
	public const int ColormapInstalled;

	public const int GrabModeSync;
	public const int GrabModeAsync;

	public const int GrabSuccess;
	public const int AlreadyGrabbed;
	public const int GrabInvalidTime;
	public const int GrabNotViewable;
	public const int GrabFrozen;

	public const int AsyncPointer;
	public const int SyncPointer;
	public const int ReplayPointer;
	public const int AsyncKeyboard;
	public const int SyncKeyboard;
	public const int ReplayKeyboard;
	public const int AsyncBoth;
	public const int SyncBoth;

	public const int RevertToNone;
	public const int RevertToPointerRoot;
	public const int RevertToParent;

	public const int Success;
	public const int BadRequest;
	public const int BadValue;
	public const int BadWindow;
	public const int BadPixmap;
	public const int BadAtom;
	public const int BadCursor;
	public const int BadFont;
	public const int BadMatch;
	public const int BadDrawable;
	public const int BadAccess;
	public const int BadAlloc;
	public const int BadColor;
	public const int BadGC;
	public const int BadIDChoice;
	public const int BadName;
	public const int BadLength;
	public const int BadImplementation;

	public const int FirstExtensionError;
	public const int LastExtensionError;

	public const int InputOutput;
	public const int InputOnly;

	public const long CWBackPixmap;
	public const long CWBackPixel;
	public const long CWBorderPixmap;
	public const long CWBorderPixel;
	public const long CWBitGravity;
	public const long CWWinGravity;
	public const long CWBackingStore;
	public const long CWBackingPlanes;
	public const long CWBackingPixel;
	public const long CWOverrideRedirect;
	public const long CWSaveUnder;
	public const long CWEventMask;
	public const long CWDontPropagate;
	public const long CWColormap;
	public const long CWCursor;

	public const int CWX;
	public const int CWY;
	public const int CWWidth;
	public const int CWHeight;
	public const int CWBorderWidth;
	public const int CWSibling;
	public const int CWStackMode;

	public const int ForgetGravity;
	public const int NorthWestGravity;
	public const int NorthGravity;
	public const int NorthEastGravity;
	public const int WestGravity;
	public const int CenterGravity;
	public const int EastGravity;
	public const int SouthWestGravity;
	public const int SouthGravity;
	public const int SouthEastGravity;
	public const int StaticGravity;

	public const int UnmapGravity;

	public const int NotUseful;
	public const int WhenMapped;
	public const int Always;

	public const int IsUnmapped;
	public const int IsUnviewable;
	public const int IsViewable;

	public const int SetModeInsert;
	public const int SetModeDelete;

	public const int DestroyAll;
	public const int RetainPermanent;
	public const int RetainTemporary;

	public const int Above;
	public const int Below;
	public const int TopIf;
	public const int BottomIf;
	public const int Opposite;

	public const int RaiseLowest;
	public const int LowerHighest;

	public const int PropModeReplace;
	public const int PropModePrepend;
	public const int PropModeAppend;

	public const int GXclear;
	public const int GXand;
	public const int GXandReverse;
	public const int GXcopy;
	public const int GXandInverted;
	public const int GXnoop;
	public const int GXxor;
	public const int GXor;
	public const int GXnor;
	public const int GXequiv;
	public const int GXinvert;
	public const int GXorReverse;
	public const int GXcopyInverted;
	public const int GXorInverted;
	public const int GXnand;
	public const int GXset;

	public const int LineSolid;
	public const int LineOnOffDash;
	public const int LineDoubleDash;

	public const int CapNotLast;
	public const int CapButt;
	public const int CapRound;
	public const int CapProjecting;

	public const int JoinMiter;
	public const int JoinRound;
	public const int JoinBevel;

	public const int FillSolid;
	public const int FillTiled;
	public const int FillStippled;
	public const int FillOpaqueStippled;

	public const int EvenOddRule;
	public const int WindingRule;

	public const int ClipByChildren;
	public const int IncludeInferiors;

	public const int Unsorted;
	public const int YSorted;
	public const int YXSorted;
	public const int YXBanded;

	public const int CoordModeOrigin;
	public const int CoordModePrevious;

	public const int Complex;
	public const int Nonconvex;
	public const int Convex;

	public const int ArcChord;
	public const int ArcPieSlice;

	public const long GCFunction;
	public const long GCPlaneMask;
	public const long GCForeground;
	public const long GCBackground;
	public const long GCLineWidth;
	public const long GCLineStyle;
	public const long GCCapStyle;
	public const long GCJoinStyle;
	public const long GCFillStyle;
	public const long GCFillRule;
	public const long GCTime;
	public const long GCStipple;
	public const long GCTileStipXOrigin;
	public const long GCTimeStopYOrigin;
	public const long GCFont;
	public const long GCSubwindowMode;
	public const long GCGraphicsExposures;
	public const long GCClipXOrigin;
	public const long GCClipYOrigin;
	public const long GCClipMask;
	public const long GCDashOffset;
	public const long GCDashList;
	public const long GCArcMode;
	public const long GCLastBit;

	public const int FontLeftToRight;
	public const int FontRightToLeft;
	public const int FontChange;

	public const int XYBitmap;
	public const int YXBitmap;
	public const int ZBitmap;

	public const int AllocNone;
	public const int AllocAll;

	public const int DoRed;
	public const int DoGreen;
	public const int DoBlue;

	public const int CursorShape;
	public const int TileShape;
	public const int StippleShape;

	public const int AutoRepeatModeOff;
	public const int AutoRepeatModeOn;
	public const int AutoRepeatModeDefault;

	public const int LedModeOff;
	public const int LedModeOn;

	public const long KBKeyClickPercent;
	public const long KBBellPercent;
	public const long KBBellPitch;
	public const long KBBellDuration;
	public const long KBLed;
	public const long KBLedMode;
	public const long KBKey;
	public const long KBAutoRepeatMode;

	public const int MappingSuccess;
	public const int MappingBusy;
	public const int MappingFailed;

	public const int MappingModifier;
	public const int MappingKeyboard;
	public const int MappingPointer;

	public const int DontPreferBlanking;
	public const int PreferBlanking;
	public const int DefaultBlanking;

	public const int DisableScreenSaver;
	public const int DisableScreenInterval;

	public const int DontAllowExposures;
	public const int AllowExposures;
	public const int DefaultExposures;

	public const int ScreenSaverReset;
	public const int ScreenSaverActive;

	public const int HostInsert;
	public const int HostDelete;

	public const int EnableAccess;
	public const int DisableAccess;

	public const int StaticGray;
	public const int GrayScale;
	public const int StaticColor;
	public const int PseudoColor;
	public const int TrueColor;
	public const int DirectColor;

	public const int LSBFirst;
	public const int MSBFirst;

	public const int XlibSpecificationRelease;

	public const int X_HAVE_UTF8_STRING;

	public const int True;
	public const int False;

	public const int QueuedAlready;
	public const int QueuedAfterReady;
	public const int QueuedAfterFlush;

	[SimpleType]
	public struct XPointer : char * {}

	[SimpleType]
	[CCode(cname = "int")]
	public struct Bool : int {}

	[SimpleType]
	[CCode(cname = "int")]
	public struct Status : int {}

	[SimpleType]
	public struct CARD32 : ulong {}

	[SimpleType]
	public struct XID : CARD32 {}

	[SimpleType]
	public struct Mask : CARD32 {}

	[SimpleType]
	public struct Atom : CARD32 {}

	[SimpleType]
	public struct VisualID : CARD32 {}

	[SimpleType]
	public struct Time : CARD32 {}

	[SimpleType]
	public struct Window : XID {}

	[SimpleType]
	public struct Drawable : XID {}

	[SimpleType]
	public struct Cursor : XID {}

	[SimpleType]
	public stuct Colormap : XID {}

	[SimpleType]
	public struct GContext : XID {}

	[SimpleType]
	public struct KeySym : XID {}

	[SimpleType]
	public struct KeyCode : uchar {}

	[SimpleType]
	public struct wchar_t : ulong {}

	public struct XSetWindowAttributes {
		Pixmap background_pixmap;
		ulong background_pixel;
		Pixmap border_pixmap;
		ulong border_pixel;
		int bit_gravity;
		int win_gravity;
		int backing_store;
		ulong backing_planes;
		ulong backing_pixel;
		bool save_under;
		long event_mask;
		long do_not_propagate_mask;
		bool override_redirect;
		Colormap colormap;
		Cursor cursor;
	}

	
}
