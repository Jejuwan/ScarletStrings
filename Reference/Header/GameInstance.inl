template <typename T>

shared_ptr<T> CGameInstance::Clone_GameObject(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	return dynamic_pointer_cast<T>(Clone_GameObject(_eScene, _wstrPrototypeTag, _arg));
}

template <typename T>
shared_ptr<T> CGameInstance::Clone_Component(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	return dynamic_pointer_cast<T>(Clone_Component(_eScene, _wstrPrototypeTag, _arg));
}
