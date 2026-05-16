#include "app.hpp"
#include "flexbox.hpp"
#include "knob.hpp"
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
      (new FlexBox("menu"))
      ->set_margin(5.f)
      ->set_padding(5.f)
      ->add(
        (new PushButton("+"))
        ->set_text("+")
        ->set_on_click([&app](MouseButton){
            println("A");
            mut* box = static_cast<FlexBox*>(app.request("menu"));
            box->add(
              (new PushButton("yay"))
              ->set_text("yay")
            );
            box->update();
          })
        ->size(40),
        (new ToggleButton("B"))
        ->set_text("B")
        ->set_on_click([](MouseButton, bool x){println("B is {}", x);})
        ->size(40),
        (new Knob("knob"))
        ->add_dent(0.8f)
        ->size(50, 50)
      )
      ->size(200.f, 50.f)
      ->unlock_width()
    )
  );
  app.request("menu")->center_in_parent();

  app.run("obase test");
  return 0;
}

