#include <iostream>
#include <iomanip>

#include "chylatum/chylatum.hpp"
#include "CLI/CLI.hpp"

int main(int args, char* argv[]){
	float R = 2.0, ap = 2.5, q = 1;
	float xd = 0.97, xf = 0.45, xw = 0.02;

	CLI::App app {"A calculator for the number of theoretical plates.", "chylatum-cli"};
	app.add_option("--R", R, "reflux ratio");
	app.add_option("--alpha", ap, "relative volatility");
	app.add_option("--q", q, "feed-thermal-state parameter");
	app.add_option("--xd", xd, "composition of overhead product (mol %)");
	app.add_option("--xf", xf, "feed composition (mol %)");
	app.add_option("--xw", xw, "composition of bottoms product (mol %)");
	if (args == 1) {
		std::cout << app.help() << std::endl;
		return 0;
	}
	try {
		app.parse(args, argv);
	} catch (const CLI::ParseError &e) {
		return app.exit(e);
	}

	const line rl = rectOpLine(xd, R), ql = qLine(xf, q);
	const line sl = striOpLine(xw, rl, ql);
	const curve vle = curve{.alpha = ap};

	const result resultr = rectify(ql, rl, vle, 100, false);
	const result results = strip(resultr.xn, sl, vle, 100, false);

	std::cout << std::setprecision(2) << std::fixed << std::right;
	std::cout << std::setw(48) << std::setfill('=') << '\0' << std::setfill(' ') << '\n';
	std::cout << "Agruments\n------\n";
	std::cout << " R: " << R << std::setw(16) << "ap: " << ap << std::setw(16) << " q: " << q << '\n';
	std::cout << "xd: " << xd << std::setw(16) << "xf: " << xf << std::setw(16) << "xw: " << xw << '\n';
	std::cout << std::setw(48) << std::setfill('=') << "" << std::setfill(' ') << '\n';
	std::cout << "Results\n------" << std::left;
	std::cout << std::setw(30) << "\nRm (Minimum Reflux Ratio)" << ": " << std::setw(6) << minR(xd, ql, vle);
	std::cout << std::setw(30) << "\nNt (Total Number)" << ": " << resultr.n + results.n;
	std::cout << std::setw(30) << "\nNf (Feed Location)" << ": " << resultr.n+ 1;
	std::cout << std::setw(30) << "\nNr (Rectifying)" << ": " << resultr.n;
	std::cout << std::setw(30) << "\nNs (Stripping & Reboiler)" << ": " << results.n;
	std::cout << '\n' << std::setw(48) << std::setfill('=') << '\0' << std::endl;

	return 0;
}
