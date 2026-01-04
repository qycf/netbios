include inc\detour_macros.inc
.code

; 定义OwnerWndProc的钩子
DEFINE_NAMESPACE_DETOUR Overlay,OwnerWndProc,BOOL

end