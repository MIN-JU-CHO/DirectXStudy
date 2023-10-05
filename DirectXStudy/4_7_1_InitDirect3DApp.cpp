#include "d3dApp.h"
#include "d3dUtil.h"
#include <dxerr.h>

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Init() override;
	void OnResize() override;
	void UpdateScene(float dt) override;
	void DrawScene() override;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// 디버그 빌드의 경우 실행시점 메모리 점검 기능을 켠다.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InitDirect3DApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : D3DApp(hInstance)
{

}

InitDirect3DApp::~InitDirect3DApp()
{

}

bool InitDirect3DApp::Init()
{
	if (!D3DApp::Init())
		return false;

	return true;
}

void InitDirect3DApp::OnResize()
{
	DXGI_SWAP_CHAIN_DESC sd;
	// 후면 버퍼 설정
	sd.BufferDesc.Width = mClientWidth; // 창의 클라이언트 영역 크기를 사용
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60; // 디스플레이 모드 갱신율
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼 픽셀 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 4X MSAA를 사용하는가?
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		// m4xMssaaQuality는 CheckMultisampleQualityLevels()로 얻은 것이다.
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// MSAA를 사용하지 않음.
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼 렌더링
	sd.BufferCount = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
		(void**)&dxgiAdapter));

	// 드디어 IDXGIFactory 인터페이스를 얻게 되었다.
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&dxgiFactory));
	// 이제 교환 사슬을 생성한다.
	IDXGISwapChain* mSwapChain;
	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

	// 획득한 COM 인터페이스들을 해제한다 (다 사용했으므로).
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
	D3DApp::OnResize();
}

void InitDirect3DApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// 후면 버퍼를 파란색으로 지운다. Colors::Blue는 d3dUtil.h에 정의
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView,
		reinterpret_cast<const float*>(&Colors::Blue));

	// 깊이 버퍼를 1.0f로, 스텐실 버퍼를 0으로 지운다.
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 후면 버퍼를 화면에 제시한다.
	HR(mSwapChain->Present(0, 0));
}