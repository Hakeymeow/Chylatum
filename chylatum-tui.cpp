#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <ftxui/dom/flexbox_config.hpp>
#include <string>
#include <format>
#include <utility>

#include "chylatum/chylatum.hpp"

int main(){

    std::string R_s = "2.0", q_s = "1.0", ap_s = "2.5", xd_s = "0.97", xf_s = "0.45", xw_s = "0.02", Nt_s, Nf_s, Nr_s, Ns_s, Rm_s;
    float R, q, ap, xd, xf, xw;

    auto calculate = [&]{
        using namespace std;
        if (!R_s.length() || !q_s.length() || !ap_s.length() || !xd_s.length() || !xf_s.length() || !xw_s.length())
            return;
        R = stof(R_s), q = stof(q_s), ap = stof(ap_s);
        xd = stof(xd_s), xf = stof(xf_s), xw = stof(xw_s);

        line rl = rectOpLine(xd, R), ql = qLine(xf, q);
        line sl = striOpLine(xw, rl, ql);
        curve vle = curve{ .alpha = ap };

        float Rm = minR(xd, ql, vle);
        result rresult = rectify(ql, rl, vle, 100, false);
        result sresult = strip(rresult.xn, sl, vle, 100, false);
        Rm_s = to_string(Rm);
        Nt_s = R > Rm ? to_string(rresult.n + sresult.n) : "inf";
        Nf_s = R > Rm ? to_string(rresult.n + 1) : "inf";
        Nr_s = R > Rm ? to_string(rresult.n) : "inf";
        Ns_s = R > Rm ? to_string(sresult.n) : "inf";
    };

    auto newInput = [&calculate](std::string& s){
        ftxui::Component input = ftxui::Input(&s);
        ftxui::Decorator inputStyle = ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) | ftxui::border;
        ftxui::ComponentDecorator wrap = ftxui::CatchEvent([&s, &calculate](ftxui::Event event){
            if (event == ftxui::Event::Escape){
                s.clear();
                return true;
            }
            if (event == ftxui::Event::ArrowUp || event == ftxui::Event::ArrowDown
                || event == ftxui::Event::ArrowLeft || event == ftxui::Event::ArrowRight 
                || event == ftxui::Event::Backspace || event == ftxui::Event::Tab || event == ftxui::Event::TabReverse
            ) return false;

            if (!event.character().length()) return true;
            switch (event.character().back()) {
                case '0': case '1': case '2': case '3': case '4': case '5':
                case '6': case '7': case '8': case '9': 
                    return false;
                case '.':
                    return !s.length() || s.find('.') != std::string::npos;
                case '\n': case ' ':
                    calculate();
                    return true;
                default:
                    return true;
            }
        });
        return input | wrap | inputStyle;
    };
    ftxui::Component R_i = newInput(R_s), q_i = newInput(q_s), ap_i = newInput(ap_s);
    ftxui::Component xd_i = newInput(xd_s), xf_i = newInput(xf_s), xw_i = newInput(xw_s);
    ftxui::Component inputComponents = ftxui::Container::Vertical({
        R_i, q_i, ap_i, xd_i, xf_i, xw_i
    });

    ftxui::Component renderer = ftxui::Renderer(inputComponents, [&]{

        auto newSepaDouble = [](){
            return ftxui::separatorDouble() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 64);
        };
        auto newSepa = [](){
            return ftxui::separator() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 16);
        };
        auto resultField = [](const std::string des, const std::string& res){
            return ftxui::text(std::format("{: <30} : {: <15}", des, res));
        };

        ftxui::Element title = ftxui::text("CHYLATUM") | ftxui::bold | ftxui::italic | ftxui::center;
        ftxui::Element guide = ftxui::text("TAB: move    ESC: clear    ENTER/SPACE: calculate") | ftxui::italic | ftxui::dim | ftxui::center;
        ftxui::Element arguments = ftxui::vbox({
            ftxui::text("Arguments") | ftxui::italic,
            newSepa(),
            ftxui::hbox({
                ftxui::hbox({ftxui::text(" R: "), R_i->Render()}),
                ftxui::hbox({ftxui::text(" q: "), q_i->Render()}),
                ftxui::hbox({ftxui::text("ap: "), ap_i->Render()}),
            }),
            ftxui::hbox({
                ftxui::hbox({ftxui::text("xd: "), xd_i->Render()}),
                ftxui::hbox({ftxui::text("xf: "), xf_i->Render()}),
                ftxui::hbox({ftxui::text("xw: "), xw_i->Render()})
            })
        });
        ftxui::Element results = ftxui::vbox({
            ftxui::text("Results") | ftxui::italic,
            newSepa(),
            resultField("Rm (Minimum Reflux Ratio)", Rm_s),
            resultField("Nt (Total Number)", Nt_s),
            resultField("Nf (Feed Location)", Nf_s),
            resultField("Nr (Rectifying)", Nf_s),
            resultField("Ns (Stripping and Reboiler)", Ns_s)
        });
        ftxui::Element mainBox = ftxui::vbox({
            title,
            newSepaDouble(),
            arguments,
            newSepaDouble(),
            results,
            newSepaDouble(),
            guide
        });
        return ftxui::vbox(ftxui::filler(), ftxui::hbox(ftxui::filler(), std::move(mainBox), ftxui::filler()), ftxui::filler());
    });

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.Loop(renderer);
    return 0;
}
