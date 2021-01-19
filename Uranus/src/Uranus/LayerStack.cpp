#include "urpch.h"
#include "LayerStack.h"

Uranus::LayerStack::LayerStack()
{
	_layerInsert = _layers.begin();
}

Uranus::LayerStack::~LayerStack()
{
	for (Layer* layer : _layers)
		delete layer;
}

void Uranus::LayerStack::PushLayer(Layer* layer)
{
	_layerInsert = _layers.emplace(_layerInsert, layer);
}

void Uranus::LayerStack::PushOverlay(Layer* overlay)
{
	_layers.push_back(overlay);
}

void Uranus::LayerStack::PopLayer(Layer* layer)
{
	auto item = std::find(_layers.begin(), _layers.end(), layer);
	if (item != _layers.end()) {
		layer->OnDetach();
		_layers.erase(item);
		_layerInsert--;
	}
}

void Uranus::LayerStack::PopOverlay(Layer* overlay)
{
	auto item = std::find(_layers.begin(), _layers.end(), overlay);
	if (item != _layers.end()) {
		overlay->OnDetach();
		_layers.erase(item);
	}
}
