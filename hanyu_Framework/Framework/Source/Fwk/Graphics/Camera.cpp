#include "Fwk/Graphics/Camera.h"

//プロジェクション行列用
float Camera::m_defaultViewWidth = 0.0f;
float Camera::m_defaultViewHeight = 0.0f;
float Camera::m_defaultNearZ = 0.0f;
float Camera::m_defaultFarZ = 1000.0f;

Camera::Camera() 
    : m_isActive(true)
    , m_name("main")
    , m_targetLayers()
    , m_order(0)
    , m_position(0.0f,0.0f)
    , m_viewWidth(m_defaultViewWidth)
    , m_viewHeight(m_defaultViewHeight)
    , m_nearZ(m_defaultNearZ)
    , m_farZ(m_defaultFarZ)
{
    ;
}

Camera::~Camera()
{
}

void Camera::SetPosition(const Vector2f& position) {
    m_position = position;
}
void Camera::SetPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
}
Vector2f Camera::GetPosition()const {
    return m_position;
}
DirectX::XMMATRIX Camera::GetViewMatrix()const {

	auto eyePos = DirectX::XMVectorSet(m_position.x, m_position.y, -10.0f, 0.0f);
	auto targetPos = DirectX::XMVectorSet(m_position.x, m_position.y, 0.0f, 0.0f);;
	auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    return  DirectX::XMMatrixLookAtLH(eyePos, targetPos, upward);
}
void Camera::SetOrtho(float viewWidth, float viewHeight, float nearZ, float farZ) {
    m_viewWidth = viewWidth;
    m_viewHeight = viewHeight;
    m_nearZ = nearZ;
    m_farZ = farZ;
}
DirectX::XMMATRIX Camera::GetOrthiMatrix()const {
    return DirectX::XMMatrixOrthographicLH(m_viewWidth, m_viewHeight, m_nearZ, m_farZ);
}
void Camera::SetDefaultOrtho(float viewWidth, float viewHeight, float nearZ, float farZ)
{
    m_defaultViewWidth = viewWidth;
    m_defaultViewHeight = viewHeight;
    m_defaultNearZ = nearZ;
    m_defaultFarZ = farZ;
}

void Camera::SetActive(bool isActive) {
    m_isActive = isActive;
}
bool Camera::IsActive()const {
    return m_isActive;
}

//描画対象のレイヤーを加える
void Camera::AddRenderLayer(const string& layerName) {
    if (ContainsRenderLayer(layerName)) {
        return;
    }
    m_targetLayers.push_back(layerName);
}

//描画対象のレイヤーを外す
void Camera::RemoveRenderLayer(const string& layerName) {
    
    int index = -1;
    for (int i = 0; i < m_targetLayers.size(); ++i) {
        if (m_targetLayers[i]._Equal(layerName)) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        return;
    }

    m_targetLayers.erase(m_targetLayers.begin() + index);
}

//描画対象のレイヤーを取得する
const std::vector<string>& Camera::GetRenderLayers()const {
    return m_targetLayers;
}

//指定したレイヤーを描画対象としているかを返す
bool Camera::ContainsRenderLayer(const string& layerName)const {
    for (int i = 0; i < m_targetLayers.size(); ++i) {
        if (m_targetLayers[i]._Equal(layerName)) {
            return true;
        }
    }
    return false;
}


void Camera::SetName(const string& name) {
    m_name = name;
}

const string& Camera::GetName()const {
    return m_name;
}

//表示(描画)順
void Camera::SetOrder(int order) {
    m_order = order;
}

int Camera::GetOrder()const {
    return m_order;
}
