#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface

int created = 0;

void addButton(const std::string& text, client::EventListener* callback)
{
	client::HTMLButtonElement* button = static_cast<client::HTMLButtonElement*>(client::document.createElement("button"));
	button->set_innerHTML(text.c_str());
	if (callback)
		button->addEventListener("click", callback);
	++created;
	client::document.get_body()->appendChild(button);
}

std::string randomColor()
{
	std::string color="#";
	const std::string digits = "0123456789abcdef";
	for (int i=0; i<6; i++)
		color += digits[rand()%16];
	return color;
}

void addBox()
{
	client::HTMLElement* box = static_cast<client::HTMLElement*>(client::document.createElement("div"));
	std::string color = randomColor();
	box->get_style()->set_backgroundColor(color.c_str());
	box->get_style()->set_display("inline-block");
	box->get_style()->set_height("50px");
	box->get_style()->set_width("100px");
	client::document.get_body()->appendChild(box);
}

void addMousePosition()
{
	client::HTMLElement* text = static_cast<client::HTMLElement*>(client::document.createElement("h5"));
	text->get_style()->set_backgroundColor("lightgrey");
	text->get_style()->set_display("inline-block");
	text->get_style()->set_height("50px");
	text->get_style()->set_width("100px");
	text->set_textContent("uninitialized");
	auto setMousePosition = [text](client::MouseEvent* e) -> void
	{
		if (text)
			text->set_textContent((std::to_string((int)e->get_x()) +", "+ std::to_string((int)e->get_y())) .c_str());
	};
	client::document.addEventListener("mousemove", cheerp::Callback(setMousePosition));
	client::document.get_body()->appendChild(text);
}

void createMainButton();

void buttonCreator()
{
	switch (created % 6)
	{
	case 0:
		addButton("Add a button", cheerp::Callback(buttonCreator));
		break;
	case 1:
		addButton("Reset", cheerp::Callback([](){
			auto body = client::document.get_body();
			client::Node* last = body->get_lastChild();
			while (last)
			{
				body->removeChild(last);
				last = body->get_lastChild();
			}
			createMainButton();
			}));
		break;
	case 2:
		addButton("Add box", cheerp::Callback(addBox));
		break;
	case 3:
		addButton("Double boxes!", cheerp::Callback([](){
			auto body = client::document.get_body();
			auto cloneIfDiv = [body](client::Node* node){
					if (((client::HTMLElement*)node)->get_tagName() == new client::String("DIV"))
						body->appendChild(node->cloneNode());
			};
			client::Node* first = body->get_firstChild();
			client::Node* last = body->get_lastChild();
			do {
				cloneIfDiv(first);
				first = first->get_nextSibling();
			} while (first != last);
			cloneIfDiv(last);
			}));
		break;
	case 4:
		addButton("Fake button", nullptr);
		break;
	case 5:
		addButton("Track mouse", cheerp::Callback(addMousePosition));
		break;
	}
}

void createMainButton()
{
	created = 0;
	buttonCreator();
}

//This function will be called only after the DOM is fully loaded
void loadCallback()
{
	createMainButton()
}

void webMain()
{
	client::document.addEventListener("DOMContentLoaded",cheerp::Callback(loadCallback));
}
