#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>
using namespace std;

#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//描画用カメラ(2D用）
//本来画角等設定できるべきだが、
//とりあえず位置のみ

class Camera 
{
public:

    Camera();
    ~Camera();

    void SetPosition(const Vector2f& position);
    void SetPosition(float x, float y);
    Vector2f GetPosition()const;
    DirectX::XMMATRIX GetViewMatrix()const;

    void SetOrtho(float viewWidth, float viewHeight, float nearZ, float farZ);
    DirectX::XMMATRIX GetOrthiMatrix()const;

    static void SetDefaultOrtho(float viewWidth, float viewHeight, float nearZ, float farZ);

    void SetActive(bool isActive);
    bool IsActive()const;

    //描画対象のレイヤーを加える
    void AddRenderLayer(const string& layerName);
    //描画対象のレイヤーを外す
    void RemoveRenderLayer(const string& layerName);
    //描画対象のレイヤーを取得する
    const std::vector<string>& GetRenderLayers()const;
    //指定したレイヤーを描画対象としているかを返す
    bool ContainsRenderLayer(const string& layerName)const;

    //名前
    void SetName(const string& name);
    const string& GetName()const;

    //表示(描画)順
    void SetOrder(int order);
    int GetOrder()const;

private:

    bool m_isActive;
    string m_name;
    std::vector<string> m_targetLayers;
    int m_order;

    Vector2f m_position;

    //プロジェクション行列用
    float m_viewWidth;
    float m_viewHeight;
    float m_nearZ;
    float m_farZ;

private:
    //プロジェクション行列用
    static float m_defaultViewWidth;
    static float m_defaultViewHeight;
    static float m_defaultNearZ;
    static float m_defaultFarZ;
};