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
	// ����� ������ ��� ������� �޸� ���� ����� �Ҵ�.
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
	// �ĸ� ���� ����
	sd.BufferDesc.Width = mClientWidth; // â�� Ŭ���̾�Ʈ ���� ũ�⸦ ���
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60; // ���÷��� ��� ������
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ���� �ȼ� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 4X MSAA�� ����ϴ°�?
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		// m4xMssaaQuality�� CheckMultisampleQualityLevels()�� ���� ���̴�.
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// MSAA�� ������� ����.
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���� ������
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

	// ���� IDXGIFactory �������̽��� ��� �Ǿ���.
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&dxgiFactory));
	// ���� ��ȯ �罽�� �����Ѵ�.
	IDXGISwapChain* mSwapChain;
	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

	// ȹ���� COM �������̽����� �����Ѵ� (�� ��������Ƿ�).
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
	D3DApp::OnResize();
}

void InitDirect3DApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// �ĸ� ���۸� �Ķ������� �����. Colors::Blue�� d3dUtil.h�� ����
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView,
		reinterpret_cast<const float*>(&Colors::Blue));

	// ���� ���۸� 1.0f��, ���ٽ� ���۸� 0���� �����.
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �ĸ� ���۸� ȭ�鿡 �����Ѵ�.
	HR(mSwapChain->Present(0, 0));
}