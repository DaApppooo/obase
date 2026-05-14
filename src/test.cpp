#include "app.hpp"
#include "layout.hpp"
#include "push_button.hpp"
#include "toggle_button.hpp"
#include "widget.hpp"

int main()
{
  App app;
  app.init(
    (new Layout("window"))
    ->add(
      (new ToggleButton("btn"))
      ->set_text("Click me !")
      ->set_on_click([](MouseButton, bool){println("Helloooo !");})
      ->place(0, 0, 100, 30)
      ->add_flag(W_LOCKED)
      ->add_flag(H_LOCKED),
      (new PushButton("btn2"))
      ->set_text("Push button !")
      ->set_on_click([](MouseButton){println("Hi !!!");})
      ->add_flag(W_LOCKED | H_LOCKED)
      ->place(0, 0, 100, 30)
    )
  );
  app.request("btn")->center_in_parent();
  app.request("btn2")->put_under(app.request("btn"), CENTER_X, 5.f);

  app.run("obase test");
  return 0;
}

