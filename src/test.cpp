#include "app.hpp"
#include "context_menu.hpp"
#include "flexbox.hpp"
#include "item_list.hpp"
#include "knob.hpp"
#include "layout.hpp"
#include "menu.hpp"
#include "numbox.hpp"
#include "push_button.hpp"
#include "scroll_bar.hpp"
#include "split_proxy.hpp"
#include "toggle_button.hpp"
#include "toggle_icon.hpp"
#include "widget.hpp"
#include "slider.hpp"

int main()
{
  App app;
  app.init(
    (new Layout("window"))
    ->add(
      (new SplitProxy("list.proxy"))
      ->add(
        (new ItemList("list"))
        ->add("a")
        ->add("b")
        ->add("c")
        ->size(100.f, VARIABLE_SIZE)
      )
      ->size(100, 20),
      (new ScrollBar("right"))
      ->orient(VERTICAL)
      ->size(20, VARIABLE_SIZE),
      (new ScrollBar("top"))
      ->orient(HORIZONTAL)
      ->size(VARIABLE_SIZE, 20)
    )
  );
  let proxy = app.request("list.proxy")
    ->center_in_parent();
  app.request("list")
    ->top_in(proxy)
    ->left_in(proxy, 0, true)
    ->right_in(proxy, 0, true);
  mut right = app.request("right")
    ->right_in(app.root.get(), 0)
    ->top_in(app.root.get(), 0, true)
    ->bottom_in(app.root.get(), 0, true);
  mut top = app.request("top")
    ->top_in(app.root.get(), 0)
    ->left_in(app.root.get(), 0, true);
  top->make_anchor(
    [&]{ return Anchor().anchor(top, RIGHT).on(right, LEFT).resizable(); }
  );
  
  
  app.run("obase test");
  return 0;
}

