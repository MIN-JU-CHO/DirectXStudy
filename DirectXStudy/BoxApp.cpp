//****************************************************************************
// BoxDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
// ���� �ִ� ������ ������
// ���� ���:
// ���� ��ư�� ���� ä�� ���콺�� �����̸� ���ڰ� ȸ��
// ������ ��ư�� ���� ä�� ���콺�� �����̸� ���ڰ� Ȯ��/���
//****************************************************************************

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"


struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class BoxApp : public D3DApp
{
public:
	BoxApp(HINSTANCE hInstance);
	~BoxApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();

private:
	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;

	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;

	ID3D11InputLayout* mInputLayout;

	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// ����� ������ ��� ������� �޸� ���� ����� Ȱ��ȭ�Ѵ�.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	BoxApp theApp(hInstance);

	if (!theApp.Init())
	{
		return 0;
	}

	return theApp.Run();
}
// ������ ����
BoxApp::BoxApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mBoxVB(0), mBoxIB(0), mFX(0), mTech(0),
	mfxWorldViewProj(0), mInputLayout(0),
	mTheta(1.5f * MathHelper::Pi), mPhi(0.25f * MathHelper::Pi), mRadius(5.0f)
{
	// window ���� �⺻�� "D3D11 Application" -> "Box Demo" �� ����
	mMainWndCaption = L"Box Demo";

	// ���� �ʱ�ȭ ���� ���� ����ü ������� �ʱ�ȭ
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	// ���� �ʱ�ȭ ���� ���� ��� ������� ������ķ� �ʱ�ȭ
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}

BoxApp::~BoxApp()
{
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);
}

bool BoxApp::Init()
{
	if (!D3DApp::Init())
	{
		return false;
	}

	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();

	return true;
}

void BoxApp::OnResize()
{
	D3DApp::OnResize();

	// â�� ũ�Ⱑ �������Ƿ� ��Ⱦ�� ����(AspectRatio())�ϰ�
	// ���� ����� �ٽ� ���(XMMatrixPerspectiveFovLH())�Ѵ�.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi,
		AspectRatio(), 1.0f, 1000.0f);
	// ����� ������� mProj�� update
	XMStoreFloat4x4(&mProj, P);
}

void BoxApp::UpdateScene(float dt)
{
	// ���� ��ǥ�� ��ī��Ʈ ��ǥ�� ��ȯ�Ѵ�.
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// �þ� ����� �����Ѵ�.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f); // ���̹Ƿ� w = 1.0f
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	// �þ� ��� ����� ������� mView�� update
	XMStoreFloat4x4(&mView, V);
}

void BoxApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView,
		reinterpret_cast<const float*>(&Colors::Blue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ���� ����ü �Է� ��ġ�� ����, �⺻���� ������ ����
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ���� ���� �Է� ���Կ� ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	// ���� ���� ���������ο� ���� - �Է� ������ �ܰ�
	md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	// ����� ����
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * view * proj;
	// ȿ�� ��ü�� ��� ���� ����(ex wvp���) update 
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		// ���� 36���� �׸���.
		md3dImmediateContext->DrawIndexed(36, 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// 1 �ȼ��� 4���� 1��(degree ����) �� �ǰ� �Ѵ�.
		// ������ �ȼ� �� ���� * 0.25f
		float dx = XMConvertToRadians(
			0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(
			0.25f * static_cast<float>(y - mLastMousePos.y));
		// ���콺 �Է¿� ������ ������ ���� �ֺ��� �˵� ī�޶� �����Ѵ�.
		mTheta += dx;
		mPhi += dy;
		// ������ mPhi�� �����Ѵ�.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// 1 �ȼ��� ����� 0.005 ������ �ǰ� �Ѵ�.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);
		// ���콺 �밢�� �Է¿� �����ؼ� �˵� ī�޶��� �������� �����Ѵ�.
		mRadius += dx - dy;
		// ������ ���� ����
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}
	// ��ǥ update
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void BoxApp::BuildGeometryBuffers()
{
	// ���� ���۸� �����Ѵ�.
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), (const float*)&Colors::Green },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), (const float*)&Colors::Blue },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), (const float*)&Colors::Yellow },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), (const float*)&Colors::Cyan },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), (const float*)&Colors::Magenta }
	};
	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth = sizeof(Vertex) * 8; // ������ü�� �� ������ 8��
	vbd.Usage = D3D11_USAGE_IMMUTABLE; // GPU Read Only
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���� ����
	vbd.CPUAccessFlags = 0; // GPU Read Only
	vbd.MiscFlags = 0; // ���� ����
	vbd.StructureByteStride = 0;

	// D3D11_SUBRESOURCE_DATA ���� (���� �ʱ�ȭ �ڷ�)
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));
	// ���Ŀ� DrawScene() �� ȣ���Ͽ� �Է� ���Կ� ���´�.

	// ���� ���۸� �����Ѵ�.
	UINT indices[] = {
		// ������ �ո�
		0, 1, 2,
		0, 2, 3,
		// �޸�
		4, 6, 5,
		4, 7, 6,
		// ���� ��
		4, 5, 1,
		4, 1, 0,
		// ������ ��
		3, 2, 6,
		3, 6, 7,
		// ����
		1, 5, 6,
		1, 6, 2,
		// �ظ�
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.ByteWidth = sizeof(UINT) * 36; // �� 12���� ���� 3���� �� 36��
	ibd.Usage = D3D11_USAGE_IMMUTABLE; // GPU Read Only
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // ���� ����
	ibd.CPUAccessFlags = 0; // GPU Read Only
	ibd.MiscFlags = 0; // ���� ����
	ibd.StructureByteStride = 0;

	// D3D11_SUBRESOURCE_DATA ���� (���� �ʱ�ȭ �ڷ�)
	D3D11_SUBRESOURCE_DATA iinitData;
	vinitData.pSysMem = indices;

	// ���� ���۸� ����
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
}

void BoxApp::BuildFX()
{
	// ȿ�� ������
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0,
		"fx_5_0", shaderFlags, 0, 0,
		&compiledShader, &compilationMsgs, 0);
	// compilationMsgs ������ ��� �޽����� ����Ǿ����� ���� �ִ�.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// compilationMsgs �� ���� �޽����� �����ٰ� �ص�
	// ������ ���� ���θ� ��������� �����ϴ� ���� �ٶ����ϴ�.
	if (FAILED(hr))
	{
		//assert(true);
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(
		compiledShader->GetBufferPointer(),
		compiledShader->GetBufferSize(),
		0, md3dDevice, &mFX));
	// �����ϵ� ���̴� �ڷḦ �� ��������Ƿ� ���� (mFX �� �Ҹ��ڿ��� ��������)
	ReleaseCOM(compiledShader);
	// ȿ�� ��� ���� ���� ���� �̸� ������ ���� ����
	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void BoxApp::BuildVertexLayout()
{
	// ���� �Է� ��ġ�� �����.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	// �Է� ��ġ ��ü�� �����Ѵ�.
	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.
		pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout));
	// ���Ŀ� DrawScene()���� �Է¹�ġ�� ���´�.
}