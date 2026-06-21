#include "app.hpp"
#include "item_list.hpp"
#include "layout.hpp"
#include "split_proxy.hpp"
#include "sys_settings.hpp"
#include "text_box.hpp"
#include "toggle_icon.hpp"
#include "widget.hpp"
#include "resizable_scrollbar.hpp"

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
      ->size(100, 30),
      (new ResizableScrollBar("right"))
      ->orient(VERTICAL)
      ->size(20, VARIABLE_SIZE),
      (new ScrollBar("top"))
      ->orient(HORIZONTAL)
      ->size(VARIABLE_SIZE, 20),
      (new ToggleIcon("icon", [](bool x){ println("{}", x); }))
      ->when_off(ICON_CLOSE, WHITE)
      ->when_on(ICON_FULLSCREEN, RED)
      ->size(20, 20),
      (new MonoTextBox("txt"))
      ->set_on_validate(
        [](const std::string& txt)
        { println("hi {}", txt); }
      )
      ->size(100, 30)
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
  mut icon = app.request("icon")->right_of(proxy, CENTER_Y);
  mut mono_txt = app.request("txt")->right_of(icon, CENTER_Y);
  
  
  
  app.run("obase test");
  return 0;
}

