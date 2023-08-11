#ifndef __FLASH
#define __FLASH


//IID_IShockwaveFlash

#ifdef __cplusplus
extern "C"{
#endif 

EXTERN_C const GUID LIBID_ShockwaveFlashObjects ;
EXTERN_C const GUID IID_IShockwaveFlash ;
EXTERN_C const GUID DIID__IShockwaveFlashEvents ;

DEFINE_GUID(LIBID_ShockwaveFlashObjects,0xD27CDB6B, 0xAE6D, 0x11CF, 0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
DEFINE_GUID(IID_IShockwaveFlash,		0xD27CDB6C, 0xAE6D, 0x11CF, 0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
DEFINE_GUID(DIID__IShockwaveFlashEvents,0xD27CDB6D, 0xAE6D, 0x11CF, 0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

#ifdef __cplusplus
}
#endif
//Flash interface 
interface IShockwaveFlash  : public IDispatch
{
public:
  // [-525] property ReadyState
  virtual HRESULT STDMETHODCALLTYPE get_ReadyState(long* pVal/*[out,retval]*/) = 0;
  // [124] property TotalFrames
  virtual HRESULT STDMETHODCALLTYPE get_TotalFrames(long* pVal/*[out,retval]*/) = 0;
  // [125] property Playing
  virtual HRESULT STDMETHODCALLTYPE get_Playing(BOOL* pVal/*[out,retval]*/) = 0;
  // [125] property Playing
  virtual HRESULT STDMETHODCALLTYPE set_Playing(BOOL pVal/*[in]*/) = 0;
  // [105] property Quality
  virtual HRESULT STDMETHODCALLTYPE get_Quality(int* pVal/*[out,retval]*/) = 0;
  // [105] property Quality
  virtual HRESULT STDMETHODCALLTYPE set_Quality(int pVal/*[in]*/) = 0;
  // [120] property ScaleMode
  virtual HRESULT STDMETHODCALLTYPE get_ScaleMode(int* pVal/*[out,retval]*/) = 0;
  // [120] property ScaleMode
  virtual HRESULT STDMETHODCALLTYPE set_ScaleMode(int pVal/*[in]*/) = 0;
  // [121] property AlignMode
  virtual HRESULT STDMETHODCALLTYPE get_AlignMode(int* pVal/*[out,retval]*/) = 0;
  // [121] property AlignMode
  virtual HRESULT STDMETHODCALLTYPE set_AlignMode(int pVal/*[in]*/) = 0;
  // [123] property BackgroundColor
  virtual HRESULT STDMETHODCALLTYPE get_BackgroundColor(long* pVal/*[out,retval]*/) = 0;
  // [123] property BackgroundColor
  virtual HRESULT STDMETHODCALLTYPE set_BackgroundColor(long pVal/*[in]*/) = 0;
  // [106] property Loop
  virtual HRESULT STDMETHODCALLTYPE get_Loop(BOOL* pVal/*[out,retval]*/) = 0;
  // [106] property Loop
  virtual HRESULT STDMETHODCALLTYPE set_Loop(BOOL pVal/*[in]*/) = 0;
  // [102] property Movie
  virtual HRESULT STDMETHODCALLTYPE get_Movie(BSTR* pVal/*[out,retval]*/) = 0;
  // [102] property Movie
  virtual HRESULT STDMETHODCALLTYPE set_Movie(BSTR pVal/*[in]*/) = 0;
  // [107] property FrameNum
  virtual HRESULT STDMETHODCALLTYPE get_FrameNum(long* pVal/*[out,retval]*/) = 0;
  // [107] property FrameNum
  virtual HRESULT STDMETHODCALLTYPE set_FrameNum(long pVal/*[in]*/) = 0;
  // [109] method SetZoomRect
  virtual HRESULT STDMETHODCALLTYPE SetZoomRect(long left/*[in]*/, long top/*[in]*/, 
                                                long right/*[in]*/, long bottom/*[in]*/) = 0;
  // [118] method Zoom
  virtual HRESULT STDMETHODCALLTYPE Zoom(int factor/*[in]*/) = 0;
  // [119] method Pan
  virtual HRESULT STDMETHODCALLTYPE Pan(long x/*[in]*/, long y/*[in]*/, int mode/*[in]*/) = 0;
  // [112] method Play
  virtual HRESULT STDMETHODCALLTYPE Play(void) = 0;
  // [113] method Stop
  virtual HRESULT STDMETHODCALLTYPE Stop(void) = 0;
  // [114] method Back
  virtual HRESULT STDMETHODCALLTYPE Back(void) = 0;
  // [115] method Forward
  virtual HRESULT STDMETHODCALLTYPE Forward(void) = 0;
  // [116] method Rewind
  virtual HRESULT STDMETHODCALLTYPE Rewind(void) = 0;
  // [126] method StopPlay
  virtual HRESULT STDMETHODCALLTYPE StopPlay(void) = 0;
  // [127] method GotoFrame
  virtual HRESULT STDMETHODCALLTYPE GotoFrame(long FrameNum/*[in]*/) = 0;
  // [128] method CurrentFrame
  virtual HRESULT STDMETHODCALLTYPE CurrentFrame(long* FrameNum/*[out,retval]*/) = 0;
  // [129] method IsPlaying
  virtual HRESULT STDMETHODCALLTYPE IsPlaying(BOOL* Playing/*[out,retval]*/) = 0;
  // [130] method PercentLoaded
  virtual HRESULT STDMETHODCALLTYPE PercentLoaded(long* percent/*[out,retval]*/) = 0;
  // [131] method FrameLoaded
  virtual HRESULT STDMETHODCALLTYPE FrameLoaded(long FrameNum/*[in]*/, 
                                                BOOL* loaded/*[out,retval]*/) = 0;
  // [132] method FlashVersion
  virtual HRESULT STDMETHODCALLTYPE FlashVersion(long* version/*[out,retval]*/) = 0;
  // [133] property WMode
  virtual HRESULT STDMETHODCALLTYPE get_WMode(BSTR* pVal/*[out,retval]*/) = 0;
  // [133] property WMode
  virtual HRESULT STDMETHODCALLTYPE set_WMode(BSTR pVal/*[in]*/) = 0;
  // [134] property SAlign
  virtual HRESULT STDMETHODCALLTYPE get_SAlign(BSTR* pVal/*[out,retval]*/) = 0;
  // [134] property SAlign
  virtual HRESULT STDMETHODCALLTYPE set_SAlign(BSTR pVal/*[in]*/) = 0;
  // [135] property Menu
  virtual HRESULT STDMETHODCALLTYPE get_Menu(BOOL* pVal/*[out,retval]*/) = 0;
  // [135] property Menu
  virtual HRESULT STDMETHODCALLTYPE set_Menu(BOOL pVal/*[in]*/) = 0;
  // [136] property Base
  virtual HRESULT STDMETHODCALLTYPE get_Base(BSTR* pVal/*[out,retval]*/) = 0;
  // [136] property Base
  virtual HRESULT STDMETHODCALLTYPE set_Base(BSTR pVal/*[in]*/) = 0;
  // [137] property Scale
  virtual HRESULT STDMETHODCALLTYPE get_Scale(BSTR* pVal/*[out,retval]*/) = 0;
  // [137] property Scale
  virtual HRESULT STDMETHODCALLTYPE set_Scale(BSTR pVal/*[in]*/) = 0;
  // [138] property DeviceFont
  virtual HRESULT STDMETHODCALLTYPE get_DeviceFont(BOOL* pVal/*[out,retval]*/) = 0;
  // [138] property DeviceFont
  virtual HRESULT STDMETHODCALLTYPE set_DeviceFont(BOOL pVal/*[in]*/) = 0;
  // [139] property EmbedMovie
  virtual HRESULT STDMETHODCALLTYPE get_EmbedMovie(BOOL* pVal/*[out,retval]*/) = 0;
  // [139] property EmbedMovie
  virtual HRESULT STDMETHODCALLTYPE set_EmbedMovie(BOOL pVal/*[in]*/) = 0;
  // [140] property BGColor
  virtual HRESULT STDMETHODCALLTYPE get_BGColor(BSTR* pVal/*[out,retval]*/) = 0;
  // [140] property BGColor
  virtual HRESULT STDMETHODCALLTYPE set_BGColor(BSTR pVal/*[in]*/) = 0;
  // [141] property Quality2
  virtual HRESULT STDMETHODCALLTYPE get_Quality2(BSTR* pVal/*[out,retval]*/) = 0;
  // [141] property Quality2
  virtual HRESULT STDMETHODCALLTYPE set_Quality2(BSTR pVal/*[in]*/) = 0;
  // [142] method LoadMovie
  virtual HRESULT STDMETHODCALLTYPE LoadMovie(int layer/*[in]*/, BSTR url/*[in]*/) = 0;
  // [143] method TGotoFrame
  virtual HRESULT STDMETHODCALLTYPE TGotoFrame(BSTR target/*[in]*/, long FrameNum/*[in]*/) = 0;
  // [144] method TGotoLabel
  virtual HRESULT STDMETHODCALLTYPE TGotoLabel(BSTR target/*[in]*/, BSTR label/*[in]*/) = 0;
  // [145] method TCurrentFrame
  virtual HRESULT STDMETHODCALLTYPE TCurrentFrame(BSTR target/*[in]*/, 
                                                  long* FrameNum/*[out,retval]*/) = 0;
  // [146] method TCurrentLabel
  virtual HRESULT STDMETHODCALLTYPE TCurrentLabel(BSTR target/*[in]*/, BSTR* pVal/*[out,retval]*/) = 0;
  // [147] method TPlay
  virtual HRESULT STDMETHODCALLTYPE TPlay(BSTR target/*[in]*/) = 0;
  // [148] method TStopPlay
  virtual HRESULT STDMETHODCALLTYPE TStopPlay(BSTR target/*[in]*/) = 0;
  // [151] method SetVariable
  virtual HRESULT STDMETHODCALLTYPE SetVariable(BSTR name/*[in]*/, BSTR value/*[in]*/) = 0;
  // [152] method GetVariable
  virtual HRESULT STDMETHODCALLTYPE GetVariable(BSTR name/*[in]*/, BSTR* pVal/*[out,retval]*/) = 0;
  // [153] method TSetProperty
  virtual HRESULT STDMETHODCALLTYPE TSetProperty(BSTR target/*[in]*/, int property/*[in]*/, 
                                                 BSTR value/*[in]*/) = 0;
  // [154] method TGetProperty
  virtual HRESULT STDMETHODCALLTYPE TGetProperty(BSTR target/*[in]*/, int property/*[in]*/, 
                                                 BSTR* pVal/*[out,retval]*/) = 0;
  // [155] method TCallFrame
  virtual HRESULT STDMETHODCALLTYPE TCallFrame(BSTR target/*[in]*/, int FrameNum/*[in]*/) = 0;
  // [156] method TCallLabel
  virtual HRESULT STDMETHODCALLTYPE TCallLabel(BSTR target/*[in]*/, BSTR label/*[in]*/) = 0;
  // [157] method TSetPropertyNum
  virtual HRESULT STDMETHODCALLTYPE TSetPropertyNum(BSTR target/*[in]*/, int property/*[in]*/, 
                                                    double value/*[in]*/) = 0;
  // [158] method TGetPropertyNum
  virtual HRESULT STDMETHODCALLTYPE TGetPropertyNum(BSTR target/*[in]*/, int property/*[in]*/, 
                                                    double* pVal/*[out,retval]*/) = 0;
  // [172] method TGetPropertyAsNumber
  virtual HRESULT STDMETHODCALLTYPE TGetPropertyAsNumber(BSTR target/*[in]*/, int property/*[in]*/, 
                                                         double* pVal/*[out,retval]*/) = 0;
  // [159] property SWRemote
  virtual HRESULT STDMETHODCALLTYPE get_SWRemote(BSTR* pVal/*[out,retval]*/) = 0;
  // [159] property SWRemote
  virtual HRESULT STDMETHODCALLTYPE set_SWRemote(BSTR pVal/*[in]*/) = 0;
  // [170] property FlashVars
  virtual HRESULT STDMETHODCALLTYPE get_FlashVars(BSTR* pVal/*[out,retval]*/) = 0;
  // [170] property FlashVars
  virtual HRESULT STDMETHODCALLTYPE set_FlashVars(BSTR pVal/*[in]*/) = 0;
  // [171] property AllowScriptAccess
  virtual HRESULT STDMETHODCALLTYPE get_AllowScriptAccess(BSTR* pVal/*[out,retval]*/) = 0;
  // [171] property AllowScriptAccess
  virtual HRESULT STDMETHODCALLTYPE set_AllowScriptAccess(BSTR pVal/*[in]*/) = 0;
  // [190] property MovieData
  virtual HRESULT STDMETHODCALLTYPE get_MovieData(BSTR* pVal/*[out,retval]*/) = 0;
  // [190] property MovieData
  virtual HRESULT STDMETHODCALLTYPE set_MovieData(BSTR pVal/*[in]*/) = 0;
  // [191] property inline-data
  virtual HRESULT STDMETHODCALLTYPE get_InlineData(LPUNKNOWN* ppIUnknown/*[out,retval]*/) = 0;
  // [191] property inline-data
  virtual HRESULT STDMETHODCALLTYPE set_InlineData(LPUNKNOWN ppIUnknown/*[in]*/) = 0;
  // [192] property SeamlessTabbing
  virtual HRESULT STDMETHODCALLTYPE get_SeamlessTabbing(BOOL* pVal/*[out,retval]*/) = 0;
  // [192] property SeamlessTabbing
  virtual HRESULT STDMETHODCALLTYPE set_SeamlessTabbing(BOOL pVal/*[in]*/) = 0;
};

#endif

