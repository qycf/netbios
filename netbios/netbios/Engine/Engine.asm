include inc\detour_macros.inc
.code

; 定义PostRender的钩子
DEFINE_NAMESPACE_DETOUR Engine,PostRender,JUMP

end