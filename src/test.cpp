#include "app.hpp"
#include "flexbox.hpp"
#include "knob.hpp"
#include "layout.hpp"
#include "numbox.hpp"
#include "push_button.hpp"
#include "toggle_button.hpp"
#include "widget.hpp"
#include "slider.hpp"

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
        ->size(50, 50),
        (new NumBox("MyNumBox"))
        ->range(30.0, 500.0)
        ->format(-2, 2)
        ->size(100, 40)
        ->unlock_width(),
        (new Slider("MySlider"))
        ->dent(8)
        ->orient(HORIZONTAL)
        ->size(200, 30)
      )
      ->size(200.f, 50.f)
      ->unlock_width(),
      (new Slider("vertical_slider"))
      ->add_dent(0.8)
      ->orient(VERTICAL)
      ->size(30, 200)
    )
  );
  app.request("menu")->center_in_parent();
  app.request("vertical_slider")->top_in(app.request("window"));
  app.request("vertical_slider")->left_in(app.request("window"));

  app.run("obase test");
  return 0;
}

