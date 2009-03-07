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
	public const int NotifyDetailNone;

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

	[Compact]
	[CCode(cname = "XPointer")]
	public class XPointer {}

	[SimpleType]
	[CCode(cname = "Bool")]
	public struct Bool : int {}

	[SimpleType]
	[CCode(cname = "Status")]
	public struct Status : int {}

	[SimpleType]
	[CCode(cname = "CARD32")]
	public struct CARD32 : ulong {}

	[SimpleType]
	[CCode(cname = "XID")]
	public struct XID : CARD32 {}

	[SimpleType]
	[CCode(cname = "Mask")]
	public struct Mask : CARD32 {}

	[SimpleType]
	[CCode(cname = "Atom")]
	public struct Atom : CARD32 {}

	[SimpleType]
	[CCode(cname = "VisualID")]
	public struct VisualID : CARD32 {}

	[SimpleType]
	[CCode(cname = "Time")]
	public struct Time : CARD32 {}

	[SimpleType]
	[CCode(cname = "Window")]
	public struct Window : XID {}

	[SimpleType]
	[CCode(cname = "Drawable")]
	public struct Drawable : XID {}

	[SimpleType]
	[CCode(cname = "Cursor")]
	public struct Cursor : XID {}

	[SimpleType]
	[CCode(cname = "Colormap")]
	public struct Colormap : XID {}

	[SimpleType]
	[CCode(cname = "GContext")]
	public struct GContext : XID {}

	[SimpleType]
	[CCode(cname = "KeySym")]
	public struct KeySym : XID {}

	[SimpleType]
	[CCode(cname = "KeyCode")]
	public struct KeyCode : uchar {}

	[SimpleType]
	[CCode(cname = "wchar")]
	public struct wchar_t : ulong {}

	[SimpleType]
	[CCode(cname = "GC")]
	public struct GC {}

	[SimpleType]
	[CCode(cname = "Pixmap")]
	public struct Pixmap : XID {}

	[SimpleType]
	[CCode(cname = "Font")]
	public struct Font : XID {}

	[SimpleType]
	[CCode(cname = "XPixmapFormatValues")]
	public struct XPixmapFormatValues {
		public int depth;
		public int bits_per_pixel;
		public int scanline_pad;

		[CCode(cname = "XListPixmapFormats")]
		public static XPixmapFormatValues[] get_formats_for_display(Display d);
	}

	[CCode(cname = "XColor")]
	public struct XColor {
		ulong pixel;
		ushort red;
		ushort green;
		ushort blue;
		char flags;
		char pad;
	}

	[CCode(cname = "XSetWindowAttributes")]
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

	[CCode(cname = "XWindowAttributes")]
	public struct XWindowAttributes {
		int x;
		int y;
		int width;
		int height;
		int depth;
		Visual visual;
		Window root;
		[CCode(cname = "class")] int win_class;
		int bit_gravity;
		int win_gravity;
		int backing_store;
		ulong backing_planes;
		ulong backing_pixel;
		bool save_under;
		Colormap colormap;
		bool map_installed;
		int map_state;
		long all_event_masks;
		long your_event_mask;
		long do_not_propagate_mask;
		bool override_redirect;
		Screen screen;
	}

	[CCode(cname = "XWindowChanges")]
	public struct XWindowChanges {
		int x;
		int y;
		int width;
		int height;
		Window sibling;
		int stack_mode;
	}

	[CCode(cname = "Visual")]
	public struct Visual {
		// Opaque structure?
		[CCode(cname = "XVisualIDFromVisual")]
		public VisualID get_visual_id();
	}

	[CCode(cname = "XInitThreads")]
	public Status init_threads();

	[CCode(cname = "XFree")]
	public void free(void* data);

	[Compact]
	[CCode(cname = "Display", cprefix = "", lower_case_cprefix = "",
		   free_function = "XCloseDisplay")]
	public class Display {
		[CCode(cname = "XOpenDisplay")]
		public Display(string? name = null);

		[CCode(cname = "XAllPlanes")]
		public static ulong get_all_planes();

		[CCode(cname = "XConnectionNumber")]
		public int get_connection_number();

		[CCode(cname = "XDefaultRootWindow")]
		public Window get_default_root_window();

		[CCode(cname = "XDefaultScreenOfDisplay")]
		public unowned Screen get_default_screen();

		[CCode(cname = "XScreenOfDisplay")]
		public unowned Screen get_screen_by_id(int screen_number);

		[CCode(cname = "XDisplayString")]
		public string get_display_string();

		[CCode(cname = "XMaxRequestSize")]
		public long get_max_request_size();

		[CCode(cname = "XExtendedMaxRequestSize")]
		public long get_max_extended_request_size();

		[CCode(cname = "XLastKnownRequestProcessed")]
		public ulong get_last_known_request_processed();

		[CCode(cname = "XNextRequest")]
		public ulong get_next_request_id();

		[CCode(cname = "XProtocolVersion")]
		public int get_protocol_version();

		[CCode(cname = "XProtocolRevision")]
		public int get_protocol_revision();

		[CCode(cname = "XQLength")]
		public int get_event_queue_length();

		[CCode(cname = "XScreenCount")]
		public int get_number_of_screens();

		[CCode(cname = "XServerVendor")]
		public string get_xserver_vendor_name();

		[CCode(cname = "XVendorRelease")]
		public string get_xserver_vendor_release();

		[CCode(cname = "XImageByteOrder")]
		public int get_image_byte_order();

		[CCode(cname = "XBitmapBitOrder")]
		public int get_bitmap_bit_order();

		[CCode(cname = "XBitmapUnit")]
		public int get_bitmap_scanline_unit();

		[CCode(cname = "XBitmapPad")]
		public int get_bitmap_scanline_padding();

		[CCode(cname = "XNoOp")]
		public void no_operation();

		[CCode(cname = "XSetCloseDownMode")]
		public void set_close_down_mode(int close_mode);

		[CCode(cname = "XLockDisplay")]
		public void acquire_lock();

		[CCode(cname = "XUnlockDisplay")]
		public void release_lock();

		/*
		 * How to wrap XConnectionWatchProc and
		 * XAddConnectionWatch/XRemoveConnectionWatch?
		 * (see p29, Xlib Reference manual)
		 */

		[CCode(cname = "XProcessInternalConnection")]
		public void process_internal_connection(int fd);

		[CCode(cname = "XInternalConnectionNumbers")]
		public Status get_internal_connections(ref int[] fd_return);

		[CCode(cname = "XFlush")]
		public int flush();

		// Display functions, sort of.  Really, they are Window functions.
		[CCode(cname = "XCreateWindow")]
		public Window create_window(Window parent, int x, int y,
									uint width, uint height, uint border_width,
									int depth, uint win_class, Visual vis,
									XSetWindowAttributes attribs);

		[CCode(cname = "XCreateSimpleWindow")]
		public Window create_simple_window(Window parent, int x, int y,
										   uint width, uint height,
										   uint border_width, ulong border,
										   ulong background);

		[CCode(cname = "XDestroyWindow")]
		public void destroy_window(Window win);

		[CCode(cname = "XDestroySubwindows")]
		public void destroy_subwindows(Window win);

		[CCode(cname = "XMapWindow")]
		public void map_window(Window win);

		[CCode(cname = "XMapRaised")]
		public void map_window_and_raise_window(Window win);

		[CCode(cname = "XMapSubwindows")]
		public void map_subwindows(Window win);

		[CCode(cname = "XUnmapWindow")]
		public void unmap_window(Window win);

		[CCode(cname = "XUnmapSubwindows")]
		public void unmap_subwindows(Window win);

		[CCode(cname = "XConfigureWindow")]
		public void configure_window(Window win, uint value_mask,
									 XWindowChanges changes);

		[CCode(cname = "XMoveWindow")]
		public void move_window(Window win, int x, int y);

		[CCode(cname = "XResizeWindow")]
		public void resize_window(Window win, uint width, uint height);

		[CCode(cname = "XMoveResizeWindow")]
		public void move_and_resize_window(Window win, int x, int y,
										   uint width, uint height);

		[CCode(cname = "XSetWindowBorderWidth")]
		public void set_window_border_width(Window win, uint width);

		[CCode(cname = "XRaiseWindow")]
		public void raise_window(Window win);

		[CCode(cname = "XLowerWindow")]
		public void lower_window(Window win);

		[CCode(cname = "XCirculateSubwindows")]
		public void circulate_subwindows(Window win, int direction);

		[CCode(cname = "XCirculateSubwindowsUp")]
		public void circulate_subwindows_up(Window win);

		[CCode(cname = "XCirculateSubwindowsDown")]
		public void circulate_subwindows_down(Window win);

		[CCode(cname = "XRestackWindows")]
		public void restack_windows(Window[] windows);

		[CCode(cname = "XChangeWindowAttributes")]
		public void change_window_attribs(Window win, ulong value_mask,
										  XSetWindowAttributes attribs);

		[CCode(cname = "XSetWindowBackground")]
		public void set_window_background(Window win, ulong bg_pixel);

		[CCode(cname = "XSetWindowBackgroundPixmap")]
		public void set_window_background_pixmap(Window win, Pixmap bg_pixmap);

		[CCode(cname = "XSetWindowBorder")]
		public void set_window_border(Window win, ulong border_pixel);

		[CCode(cname = "XSetWindowBorderPixmap")]
		public void set_window_border_pixmap(Window win, Pixmap border_pixmap);

		[CCode(cname = "XSetWindowColormap")]
		public void set_window_colormap(Window win, Colormap cmap);

		[CCode(cname = "XDefineCursor")]
		public void define_cursor(Window win, Cursor cur);

		[CCode(cname = "XUndefineCursor")]
		public void undefine_cursor(Window win);

		[CCode(cname = "XQueryTree")]
		public Status query_window_tree(Window win, out Window root,
										out Window parent,
										out Window[] children);

		[CCode(cname = "XGetWindowAttributes")]
		public Status get_window_attributes(Window win,
											out XWindowAttributes retval);

		[CCode(cname = "XTranslateCoordinates")]
		public bool translate_coordinates(Window src, Window dest, int src_X,
										  int src_y, out int dest_x,
										  out int dest_y, out Window child);

		[CCode(cname = "XQueryPointer")]
		public bool query_pointer(Window win, out Window root,
								  out Window child, out int root_x,
								  out int root_y, out int win_x, out int win_y,
								  out uint mask);

		[CCode(cname = "XGetWindowProperty")]
		public int get_window_property(Window win, Atom prop, long offset,
									   long length, bool delete, Atom req_type,
									   out Atom actual_type,
									   out int actual_format,
									   out ulong item_count,
									   out ulong bytes_after,
									   uchar*[] prop_ret);

		[CCode(cname = "XListProperties")]
		public Atom[] list_properties(Window win, out int count);

		[CCode(cname = "XChangeProperty")]
		public void change_property(Window win, Atom property,
									Atom type, int format, int mode,
									uchar[] data, int count);

		[CCode(cname = "XRotateWindowProperties")]
		public void rotate_properites(Window win, Atom[] properties, int npos);

		[CCode(cname = "XDeleteProperty")]
		public void delete_property(Window win, Atom prop);

		[CCode(cname = "XSetSelectionOwner")]
		public void set_selection_owner(Atom selection, Window who, Time when);

		[CCode(cname = "XGetSelectionOwner")]
		public Window get_selection_owner(Atom selection);

		[CCode(cname = "XConvertSelection")]
		public void convert_selection(Atom selection, Atom target, Atom prop,
									  Window requestor, Time when);

		[CCode(cname = "XGetGeometry")]
		public Status get_geometry(Drawable d, out Window root,
								   out int x, out int y, out int width,
								   out int height, out int border_width,
								   out int depth);

		[CCode(cname = "XInternAtom")]
		public Atom intern_atom(string atom_name, bool only_if_exists);

		[CCode(cname = "XInternAtoms")]
		public Atom intern_atoms(string[] names, bool only_if_exists,
								 Atom[] atoms);

		[CCode(cname = "XGetAtomName")]
		public string get_atom_name(Atom a);

		[CCode(cname = "XGetAtomNames")]
		public string get_atom_names(Atom[] atoms, string[] names);

		// Pixmap functions
		[CCode(cname = "XCreatePixmap")]
		public Pixmap create_pixmap(Drawable d, uint width, uint height,
									uint depth);

		[CCode(cname = "XFreePixmap")]
		public void free_pixmap(Pixmap p);

		[CCode(cname = "XCreateFontCursor",
			   cheader_filename = "X11/cursorfont.h")]
		public Cursor create_font_cursor(uint shape);

		[CCode(cname = "XCreateGlyphCursor")]
		public Cursor create_glyph_cursor(Font source_font, Font mask_font,
										  uint source_char, uint mask_char,
										  XColor fg_color, XColor bg_color);

		[CCode(cname = "XCreatePixmapCursor")]
		public Cursor create_pixmap_cursor(Pixmap source, Pixmap mask,
										   XColor fg_color, XColor bg_color);

		[CCode(cname = "XQueryBestCursor")]
		public Status query_best_cursor(Drawable d, uint width, uint height,
										out uint width_return,
										out uint height_return);

		[CCode(cname = "XRecolorCursor")]
		public void recolor_cursor(Cursor cursor, XColor fg, XColor bg);

		[CCode(cname = "XFreeCursor")]
		public void free_cursor(Cursor cursor);

		[CCode(cname = "XCreateColormap")]
		public Colormap create_colormap(Window win, Visual vis, int alloc);

		[CCode(cname = "XCopyColormapAndFree")]
		public Colormap copy_and_free(Colormap cmap);

		[CCode(cname = "XFreeColormap")]
		public void free_colormap(Colormap cmap);

		[CCode(cname = "XLookupColor")]
		public Status lookup_color(Colormap cmap, string color_name,
								   out XColor exact, out XColor screen);

		[CCode(cname = "XParseColor")]
		public Status parse_color(Colormap cmap, string color_name,
								  out XColor exact);

		[CCode(cname = "XAllocColor")]
		public Status alloc_color(Colormap cmap, ref XColor screen_io);

		[CCode(cname = "XAllocNamedColor")]
		public Status alloc_named_color(Colormap cmap, string color_name,
										out XColor screen_def,
										out XColor exact_def);

		[CCode(cname = "XAllocColorCells")]
		public Status alloc_color_cells(Colormap cmap, bool contig,
										out ulong[] plane_masks,
										out ulong[] pixels);

		[CCode(cname = "XAllocColorPlanes")]
		public Status alloc_color_planes(Colormap cmap, bool contig,
										 out ulong[] pixels,
										 int nreds, int ngreens, int nblues,
										 out ulong rmask, out ulong gmask,
										 out ulong bmask);

		[CCode(cname = "XFreeColors")]
		public void free_colors(Colormap cmap, ulong[] pixels, ulong planes);

		[CCode(cname = "XStoreColor")]
		public void store_color(Colormap cmap, XColor col);

		[CCode(cname = "XStoreColors")]
		public void store_colors(Colormap cmap, XColor[] colors);

		[CCode(cname = "XStoreNamedColor")]
		public void store_named_color(Colormap cmap, string color, ulong pixel,
									  int flags);

		[CCode(cname = "XQueryColor")]
		public void query_color(Colormap cmap, ref XColor color_io);

		[CCode(cname = "XQueryColors")]
		public void query_colors(Colormap cmap, ref XColor[] defs_io);

		// No Xcms functions (yet).

		// No Graphics Context functions (yet).

	}

	[Compact]
	[CCode(cname = "Screen", free_func = "")]
	public class Screen {
		// Commented out until Vala supports multiple constructors!
		// [CCode(cname = "XDefaultScreenOfDisplay")]
		// public Screen(Display disp);

		[CCode(cname = "XScreenOfDisplay")]
		public static unowned Screen
			get_screen(Display disp, int screen_number);

		[CCode(cname = "XBlackPixelOfScreen")]
		public ulong get_black_pixel();

		[CCode(cname = "XWhitePixelOfScreen")]
		public ulong get_white_pixel();

		[CCode(cname = "XCellsOfScreen")]
		public int get_colormap_cells();

		[CCode(cname = "XDefaultColormapOfScreen")]
		public Colormap get_default_colormap();

		[CCode(cname = "XDefaultDepthOfScreen")]
		public int get_depth();

		[CCode(cname = "XDefaultGCOfScreen")]
		public GC get_default_graphics_context();

		[CCode(cname = "XDefaultVisualOfScreen")]
		public Visual get_default_visual();

		[CCode(cname = "XDoesBackingStore")]
		public int does_backing_store();

		[CCode(cname = "XDoesSaveUnders")]
		public bool does_save_unders();

		[CCode(cname = "XDisplayOfScreen")]
		public Display get_display();

		[CCode(cname = "XScreenNumberOfScreen")]
		public int get_screen_number();

		[CCode(cname = "XEventMaskOfScreen")]
		public long get_event_mask();

		[CCode(cname = "XWidthOfScreen")]
		public int get_width();

		[CCode(cname = "XWidthMMOfScreen")]
		public int get_width_in_mm();

		[CCode(cname = "XHeightOfScreen")]
		public int get_height();

		[CCode(cname = "XHeightMMOfScreen")]
		public int get_height_in_mm();

		[CCode(cname = "XMaxCmapsOfScreen")]
		public int get_max_colormap_count();

		[CCode(cname = "XMinCmapsOfScreen")]
		public int get_min_colormap_count();

		[CCode(cname = "XPlanesOfScreen")]
		public int get_planes();

		[CCode(cname = "XRootWindowOfScreen")]
		public Window get_root_window();
	}
}
