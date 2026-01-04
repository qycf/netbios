#include "../include/RayTracer.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

// 控制台输出辅助函数
void PrintVector(const char* name, const FVector& vec) {
    std::cout << name << ": (" 
              << std::fixed << std::setprecision(2) 
              << vec.X << ", " 
              << vec.Y << ", " 
              << vec.Z << ")" << std::endl;
}

void PrintHitResult(const FHitResult& hit) {
    std::cout << "\n=== 射线追踪结果 ===" << std::endl;
    std::cout << "是否碰撞: " << (hit.IsHit() ? "是" : "否") << std::endl;
    
    if (hit.IsHit()) {
        std::cout << "碰撞距离: " << std::fixed << std::setprecision(2) 
                  << hit.Distance << std::endl;
        std::cout << "碰撞时间: " << hit.Time << std::endl;
        PrintVector("碰撞位置", hit.Location);
        PrintVector("撞击点", hit.ImpactPoint);
        PrintVector("表面法线", hit.Normal);
    }
    std::cout << "==================\n" << std::endl;
}

// 示例1: 基础射线追踪
void Example_BasicRayTrace() {
    std::cout << "\n【示例1: 基础射线追踪】" << std::endl;
    
    RayTracer tracer;
    
    FVector start(0, 0, 0);
    FVector end(100, 50, 0);
    
    PrintVector("起点", start);
    PrintVector("终点", end);
    
    FHitResult hitResult;
    bool hit = tracer.LineTraceSingle(start, end, hitResult);
    
    PrintHitResult(hitResult);
}

// 示例2: 可见性检测
void Example_VisibilityCheck() {
    std::cout << "\n【示例2: 可见性检测】" << std::endl;
    
    RayTracer tracer;
    
    FVector observer(0, 0, 10);
    FVector target(100, 0, 10);
    
    PrintVector("观察者位置", observer);
    PrintVector("目标位置", target);
    
    bool visible = tracer.IsVisible(observer, target);
    
    std::cout << "可见性: " << (visible ? "可见" : "被遮挡") << std::endl;
    
    // 使用VisibilityChecker辅助类
    bool hasLOS = VisibilityChecker::HasLineOfSight(observer, target, tracer);
    float distance = VisibilityChecker::GetDistance(observer, target);
    
    std::cout << "直接视线: " << (hasLOS ? "有" : "无") << std::endl;
    std::cout << "距离: " << std::fixed << std::setprecision(2) 
              << distance << std::endl;
}

// 示例3: 视锥检测
void Example_ViewFrustumCheck() {
    std::cout << "\n【示例3: 视锥检测】" << std::endl;
    
    FVector cameraPos(0, 0, 0);
    FVector cameraForward(1, 0, 0);  // 朝向X轴正方向
    float fov = 90.0f;  // 90度视野
    
    FVector testPoints[] = {
        FVector(100, 0, 0),      // 正前方
        FVector(100, 50, 0),     // 右前方
        FVector(100, -50, 0),    // 左前方
        FVector(-100, 0, 0),     // 正后方
        FVector(70, 70, 0)       // 右前方边缘
    };
    
    PrintVector("摄像机位置", cameraPos);
    PrintVector("摄像机朝向", cameraForward);
    std::cout << "视野角度: " << fov << "°" << std::endl;
    std::cout << std::endl;
    
    for (int i = 0; i < 5; ++i) {
        bool inView = VisibilityChecker::IsInViewFrustum(
            testPoints[i], cameraPos, cameraForward, fov
        );
        
        std::cout << "测试点" << (i + 1) << ": ";
        PrintVector("", testPoints[i]);
        std::cout << "  → " << (inView ? "在视野内" : "在视野外") << std::endl;
    }
}

// 示例4: 忽略特定Actor
void Example_IgnoreActors() {
    std::cout << "\n【示例4: 忽略特定Actor】" << std::endl;
    
    RayTracer tracer;
    
    // 创建一些模拟的Actor（实际使用中应该是真实的游戏对象）
    AActor* player = new AActor();
    AActor* npc = new AActor();
    
    // 添加到忽略列表
    tracer.AddIgnoredActor(player);
    tracer.AddIgnoredActor(npc);
    
    std::cout << "已添加 2 个Actor到忽略列表" << std::endl;
    
    FVector start(0, 0, 0);
    FVector end(100, 0, 0);
    
    FHitResult hitResult;
    bool hit = tracer.LineTraceSingle(start, end, hitResult);
    
    std::cout << "追踪结果（忽略了player和npc）: " 
              << (hit ? "碰撞" : "无碰撞") << std::endl;
    
    // 清理
    tracer.ClearIgnoredActors();
    std::cout << "已清空忽略列表" << std::endl;
    
    delete player;
    delete npc;
}

// 示例5: 多点射线扫描
void Example_MultiPointScan() {
    std::cout << "\n【示例5: 多点射线扫描】" << std::endl;
    
    RayTracer tracer;
    tracer.SetDebugDraw(true);
    
    FVector center(0, 0, 0);
    float radius = 100.0f;
    int rayCount = 8;  // 8个方向
    
    std::cout << "从中心点向周围发射 " << rayCount << " 条射线" << std::endl;
    PrintVector("中心点", center);
    std::cout << "半径: " << radius << std::endl << std::endl;
    
    int hitCount = 0;
    for (int i = 0; i < rayCount; ++i) {
        float angle = (360.0f / rayCount) * i;
        float radians = angle * 3.14159f / 180.0f;
        
        FVector direction(
            cosf(radians),
            sinf(radians),
            0
        );
        
        FVector end = center + (direction * radius);
        
        FHitResult hitResult;
        bool hit = tracer.LineTraceSingle(center, end, hitResult);
        
        std::cout << "射线 " << (i + 1) << " (角度: " 
                  << std::fixed << std::setprecision(0) << angle << "°): "
                  << (hit ? "碰撞" : "无碰撞");
        
        if (hit) {
            std::cout << " - 距离: " << std::setprecision(2) << hitResult.Distance;
            hitCount++;
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n碰撞统计: " << hitCount << "/" << rayCount << std::endl;
}

int main() {
    // 设置控制台为UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
    
    std::cout << "╔════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║    射线追踪（Ray Tracing）功能演示程序             ║" << std::endl;
    std::cout << "║    提取自 Unreal Engine 游戏辅助工具               ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════╝" << std::endl;
    
    // 运行所有示例
    Example_BasicRayTrace();
    Example_VisibilityCheck();
    Example_ViewFrustumCheck();
    Example_IgnoreActors();
    Example_MultiPointScan();
    
    std::cout << "\n所有示例执行完毕！" << std::endl;
    std::cout << "\n提示: 这是独立的射线追踪实现，未连接到实际游戏引擎。" << std::endl;
    std::cout << "要在真实环境中使用，需要集成游戏引擎的碰撞检测系统。" << std::endl;
    
    std::cout << "\n按任意键退出..." << std::endl;
    system("pause >nul");
    
    return 0;
}
