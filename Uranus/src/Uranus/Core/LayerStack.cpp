#include "urpch.h"
#include "LayerStack.h"

Uranus::LayerStack::LayerStack()
{
	
}

Uranus::LayerStack::~LayerStack()
{
	for (Layer* layer : _Layers) {
		layer->OnDetach();
		delete layer;
	}
}

void Uranus::LayerStack::PushLayer(Layer* layer)
{
	_Layers.emplace(_Layers.begin() + _LayerInsertIndex, layer);
	_LayerInsertIndex++;
}

void Uranus::LayerStack::PushOverlay(Layer* overlay)
{
	_Layers.push_back(overlay);
}

void Uranus::LayerStack::PopLayer(Layer* layer)
{
	auto item = std::find(_Layers.begin(), _Layers.end(), layer);
	if (item != _Layers.end()) {
		layer->OnDetach();
		_Layers.erase(item);
		_LayerInsertIndex--;
	}
}

void Uranus::LayerStack::PopOverlay(Layer* overlay)
{
	auto item = std::find(_Layers.begin(), _Layers.end(), overlay);
	if (item != _Layers.end()) {
		overlay->OnDetach();
		_Layers.erase(item);
	}
}
