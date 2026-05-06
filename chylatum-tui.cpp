#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <format>

#include "chylatum/chylatum.hpp"

int main(){

    std::string R_s = "2.0", q_s = "1.0", ap_s = "2.5", xd_s = "0.97", xf_s = "0.45", xw_s = "0.02", Nt_s, Nf_s, Nr_s, Ns_s, Rm_s;
    float R, q, ap, xd, xf, xw;

    auto calculate = [&]{
        using namespace std;
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

    ftxui::ComponentDecorator callback = ftxui::CatchEvent([&](ftxui::Event event) {
        if (event.character().length() && event.character().back() == '\n'){
            calculate();
            return true;
        }
        return false;
    });
    ftxui::Component R_i = ftxui::Input(&R_s) | callback;
    ftxui::Component q_i = ftxui::Input(&q_s) | callback;
    ftxui::Component ap_i = ftxui::Input(&ap_s) | callback;
    ftxui::Component xd_i = ftxui::Input(&xd_s) | callback;
    ftxui::Component xf_i = ftxui::Input(&xf_s) | callback;
    ftxui::Component xw_i = ftxui::Input(&xw_s) | callback;

    ftxui::Component component = ftxui::Container::Vertical({
        R_i, q_i, ap_i, xd_i, xf_i, xw_i
    });

    ftxui::Decorator inputStyle = ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) | ftxui::border;
    ftxui::Decorator sepaStyle = ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 64);
    ftxui::Component renderer = ftxui::Renderer(component, [&]{
        return ftxui::vbox({
            ftxui::separatorDouble() | sepaStyle,
            ftxui::text("Arguments"),
            ftxui::separator() | sepaStyle,
            ftxui::hbox(
                    ftxui::text(" R: "), R_i->Render() | inputStyle, 
                    ftxui::text(" q: "), q_i->Render() | inputStyle, 
                    ftxui::text("ap: "), ap_i->Render() | inputStyle
                ),
            ftxui::hbox(
                    ftxui::text("xd: "), xd_i->Render() | inputStyle, 
                    ftxui::text("xf: "), xf_i->Render() | inputStyle, 
                    ftxui::text("xw: "), xw_i->Render() | inputStyle
                ),
            ftxui::separatorDouble() | sepaStyle,
            ftxui::text("Results"),
            ftxui::separator() | sepaStyle,
            ftxui::text(std::format("{: <30} : {: <15}", "Rm (Minimum Reflux Ratio)", Rm_s)),
            ftxui::text(std::format("{: <30} : {: <15}", "Nt (Total Number)", Nt_s)), 
            ftxui::text(std::format("{: <30} : {: <15}", "Nf (Feed Location)", Nf_s)),
            ftxui::text(std::format("{: <30} : {: <15}", "Nr (Rectifying)", Nf_s)),
            ftxui::text(std::format("{: <30} : {: <15}", "Ns (Stripping and Reboiler)", Ns_s)),
            ftxui::separatorDouble() | sepaStyle
        });
    });
    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
    return 0;
}
