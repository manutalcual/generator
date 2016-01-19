//-*- mode: c++; indent-tabs-mode: t; -*-
//
//! @brief Nombre del proyecto, bloque.
//
// Todos los derechos reservados por By Tech.
// La información aquí contenida es propiedad confidencial de la Empresa. 
// La utilización, divulgación, reproducción y distribución, total o parcial, 
// de la misma está estrictamente prohibida. Salvo acuerdo expreso con By Tech.
// Micro:         Nombre del microcontrolador
// Proyecto:      PY-XXX. Nombre del proyecto
// Program: generator Copyright (c) 2015 By Tech
// Author: Manuel Cano Muñoz
// Date: Mon Dec 14 09:38:54 2015
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
#include "generator.hh"

namespace app {


	generator::generator (sys::conf & pconf)
		: _conf (pconf),
		  _path ("")
	{
		flog ();
		sys::conf::block_t * b = _conf.find("sys/main");
		_path = b->values["path"];
		_js = b->values["js"];

		logp (sys::e_debug, "Output path: '" << _path << "'.");
	}

	generator::~generator ()
	{
	}

	bool generator::make_project () throw (generator::create)
	{
		flog ();
		logp (sys::e_debug, "== begin generation ====================================================");

		sys::conf::block_t * root = _conf.find("sys");

		if (! root)
			throw create ("Can'f find 'sys' element.'",
						  __FILE__, __LINE__);

		logp (sys::e_debug, "Root found, continuing.");

		sys::conf::block_t * main = _conf.find("sys/main");
		if (! main)
			throw create ("Can'f find 'sys/main' element.'",
						  __FILE__, __LINE__);

		if (_path != "") {
			// We don't really care about the result, it should be
			// created if doesn't exists and that's it
			sys::file_system::mkdir (_path.c_str());
		}

		logp (sys::e_debug, "Name: '" << main->values["name"] << "'.");
		logp (sys::e_debug, "\tempty: '" << main->values["empty"] << "'.");

		sys::conf::block_t * sub = main->subelements["subprojects"];
		sys::conf::block_t::mapblock_t::
			iterator itb = sub->subelements.begin();
		sys::conf::block_t::mapblock_t::
			iterator ite = sub->subelements.end();


		for (; itb != ite; ++itb) {
			logp (sys::e_debug, "Name: '" << itb->second->name << "'.");
			logp (sys::e_debug, "Element: '"
				  << itb->second->name
				  << "'.");

			if (itb->second->values["type"] == "lib") {
				create_lib (itb->second);
				create_configure_ac (itb->second);
				create_makefile_am (itb->second);
				create_sub_makefile_am (itb->second);
				create_proto (itb->second);
				create_proto_enums (itb->second);
				create_angular (itb->second);
				create_html (itb->second);
				create_db_tests (itb->second);
				create_makefile (itb->second);
				create_qtpro (itb->second);
				create_servers (itb->second);
				create_server (itb->second);
			} else if (itb->second->values["type"] == "app") {
			}
		}

		nlogp (sys::e_debug, "== begin generation ====================================================");
		logp (sys::e_debug, "====================================================== end generation ==");

 		return true;
	}

	//
	// Create classes
	//
	void generator::create_lib (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);
		conf::block_t * lib = _conf.find(gene.c_str());

		conf::block_t::mapblock_t::iterator itb = lib->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = lib->subelements.end();

		std::string tmp_path (_path + "/contracts/");
		sys::file_system::mkdir (tmp_path);

		for (; itb != ite; ++itb) {
			logp (sys::e_debug, itb->second->name);
			_ch = "";
			_cb = "";
			//_path = tmp_path + "/" + itb->second->name;
			//sys::file_system::mkdir (_path);
			create_contract_class (itb->second);
		}

		//_path = tmp_path;
	}

	void generator::create_contract_class (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();

		//conf::block_t::mapblock_t::iterator itb = item->subelements.begin();
		//conf::block_t::mapblock_t::iterator ite = item->subelements.end();
		std::string business (item->name);

		business += "Business";

		logp (sys::e_debug, "\tClass: " << item->name);

		/*
		 * HEADER
		 */

		_ch = "//-*- mode: c++; indent-tabs-mode: t; -*-\n";
		_ch += "//\n";
		_ch += "//! @brief Nombre del proyecto, bloque.\n";
		_ch += "//\n";
		_ch += "// Todos los derechos reservados por By Tech.\n";
		_ch += "// La información aquí contenida es propiedad confidencial de la Empresa. \n";
		_ch += "// La utilización, divulgación, reproducción y distribución, total o parcial, \n";
		_ch += "// de la misma está estrictamente prohibida. Salvo acuerdo expreso con By Tech.\n";
		_ch += "// Micro:         Nombre del microcontrolador\n";
		_ch += "// Proyecto:      PY-XXX. Nombre del proyecto\n";
		_ch += "// Program: generator Copyright (c) 2015 By Tech\n";
		_ch += "// \n";
		_ch += "// $Id$\n";
		_ch += "//\n";
		_ch += "// Includes\n";
		_ch += "//\n";

		_ch += "#ifndef ";
		_ch += business;
		_ch += "_h\n";
		_ch += "#define ";
		_ch += business;
		_ch += "_h\n";
		_ch += "\n";
		_ch += "\n";

		_ch += "#include <QDebug>\n";
		_ch += "#include <QByteArray>\n";
		_ch += "#include \"../bytech-hulk-proto/Hulk.pb.h\"\n";
		_ch += "#include \"../bytech-hulk-proto/base.pb.h\"\n";
		_ch += "#include \"../bytech-hulk-proto/base.enums.pb.h\"\n";

		_ch += "#include \"../bytech-hulk-entities/include/hulk_all_include.gen.h\"\n";

		_ch += "#include \"entityprocessbase.h\"\n";
		_ch += "#include \"requestparams.h\"\n";
		_ch += "#include \"bymodule.h\"\n";
		_ch += "\n";
		_ch += "\n";

		_ch += "class ";
		_ch += business;
		_ch += ": public EntityProcessBase<Bytech::Hulk::";
		_ch += item->name;
		_ch += ">\n";
		//_ch += "\n";
		_ch += "{\n";
		_ch += "protected:\n";
		_ch += "    typedef QByteArray(";
		_ch += business;
		_ch += "::*ProcessAction)(const QByteArray &params);\n";
		_ch += "    QHash<ByEnum::ActionType, ProcessAction> _processorCustomActions;\n";
		_ch += "\n";
		_ch += "public:\n";
		_ch += "    ";
		_ch += business;
		_ch += "();\n";
		_ch += "\n";

		_ch += "    QByteArray Insert (const QByteArray &params);\n";
		_ch += "    QByteArray GetAll (const QByteArray &params);\n";
		_ch += "    QByteArray GetById (const QByteArray &params);\n";
		_ch += "    QByteArray Update (const QByteArray &params);\n";
		_ch += "    QByteArray Delete (const QByteArray &params);\n";
		_ch += "\n";
		_ch += "    QByteArray CustomProcess(const QByteArray &params) {} //;\n";
		_ch += "};\n";

		_ch += "#endif // ";
		_ch += business;
		_ch += "_h\n";

		//
		// Body
		//
		_cb += "#include \"";
		_cb += business;
		_cb += ".h\"\n";
		_cb += "\n";

		//
		// Constructor
		//
		_cb += "\n";
		_cb += business;
		_cb += "::";
		_cb += business;
		_cb += "()\n";
		_cb += "{\n";
		_cb += "}\n";
		_cb += "\n";

		//
		// Insert
		//
		_cb += "\n";
		_cb += "QByteArray ";
		_cb += business;
		_cb += "::Insert (const QByteArray &params)\n";
		_cb += "{\n";
		_cb += "    qDebug() << \"Insert\";\n";
		_cb += "    QByteArray response;\n";
		_cb += "    " + item->name + " itemEntity;\n";
		_cb += "    Bytech::Hulk::" + item->name + " itemProto;\n";
		_cb += "    qDebug() << \"" + item->name + "\";\n";
		_cb += "\n";

		create_contract_fields (
			item,
			item->subelements["fields"],
			[] (size_t i, std::string & str, sys::conf::block_t * item) {

				str += "    qDebug() << \"" + item->name + "\";\n";
				if (item->name == "Entity") {
					str += "    // Entity = __Disc__\n";
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    itemEntity.set__Disc__";
					str += " (QString::fromStdString(itemProto.";
					str += sys::lower(item->name);
					str += "()));\n";
					return;
				}

				if (item->values["type"] == "nvarchar") {
					logp (sys::e_debug, "nvarchar");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (\"" + item->name + "\");\n";
					str += "    QString str" + sys::itoa(++i)
						+ " = QString::fromStdString(itemProto."
						+ sys::lower(item->name) + "());\n";
					str += "    itemEntity.set" + item->name + " (str" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "integer") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    long integer" + sys::itoa(++i) + " = 10;\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (" + sys::itoa(i) + ");\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "integer" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "bit") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    int integer" + sys::itoa(++i) + " = 10;\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (" + sys::itoa(i) + ");\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "integer" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "datetime") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    QDateTime dt" + sys::itoa(++i) + ";\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "dt" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "blob") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    char * ch" + sys::itoa(++i) + " = \"abc\";\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (ch" + sys::itoa(i) + ");\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "ch" + sys::itoa(i) + ");\n";
				} else {
					logp (sys::e_debug, "other: " + item->values["type"]);
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    long other" + sys::itoa(++i) + " = 10;\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "other" + sys::itoa(i) + ");\n";
				}
			});

		_cb += "\n";
		_cb += "    itemEntity.qxInsert();\n";
		_cb += "    response = params;\n";
		_cb += "    return response;\n";
		_cb += "}\n";
		_cb += "\n";

		//
		// GetAll
		//
		_cb += "\n";
		_cb += "QByteArray ";
		_cb += business;
		_cb += "::GetAll (const QByteArray &params)\n";
		_cb += "{\n";
		_cb += "    qDebug() << \"GetAll\";\n";
		_cb += "    QByteArray response;\n";
		_cb += "    " + item->name + " itemEntity;\n";
		_cb += "    Bytech::Hulk::" + item->name + " itemProto;\n";		
		_cb += "    qDebug() << \"" + item->name + "\";\n";
		_cb += "\n";
		_cb += "    long lCount = 0;\n";
		_cb += "    //itemEntity.qxCount (lCount);\n";
		_cb += "    qDebug() << \"Count: \" << lCount << \".\";\n";
		_cb += "\n";
		/*
		create_contract_fields (
			item,
			item->subelements["fields"],
			[] (std::string & str, sys::conf::block_t * item) {
				str += "    // type '" + item->values["type"] + "'\n";
				str += "    itemEntity.set" + item->name;
				str += "       (QString::fromStdString(temProto.";
				str += sys::lower(item->name);
				str += "()));\n";
			});
		*/
		_cb += "    response = params;\n";
		_cb += "    return response;\n";
		_cb += "}\n";
		_cb += "\n";

		//
		// GetById
		//
		_cb += "\n";
		_cb += "QByteArray ";
		_cb += business;
		_cb += "::GetById (const QByteArray &params)\n";
		_cb += "{\n";
		_cb += "    qDebug() << \"GetAll\";\n";
		_cb += "    QByteArray response;\n";
		_cb += "    " + item->name + " itemEntity;\n";
		_cb += "    Bytech::Hulk::" + item->name + " itemProto;\n";		
		_cb += "    qDebug() << \"" + item->name + "\";\n";
		_cb += "\n";
		_cb += "    long lCount = 0;\n";
		_cb += "    //itemEntity.qxCount (lCount);\n";
		_cb += "    qDebug() << \"Count: \" << lCount << \".\";\n";
		_cb += "\n";
		/*
		create_contract_fields (
			item,
			item->subelements["fields"],
			[] (std::string & str, sys::conf::block_t * item) {
				str += "    // type '" + item->values["type"] + "'\n";
				str += "    itemEntity.set" + item->name;
				str += "       (QString::fromStdString(temProto.";
				str += sys::lower(item->name);
				str += "()));\n";
			});
		*/
		_cb += "    response = params;\n";
		_cb += "    return response;\n";
		_cb += "}\n";
		_cb += "\n";

		//
		// Update
		//
		_cb += "\n";
		_cb += "QByteArray ";
		_cb += business;
		_cb += "::Update (const QByteArray &params)\n";
		_cb += "{\n";
		_cb += "    qDebug() << \"GetAll\";\n";
		_cb += "    QByteArray response;\n";
		_cb += "    " + item->name + " itemEntity;\n";
		_cb += "    Bytech::Hulk::" + item->name + " itemProto;\n";		
		_cb += "    qDebug() << \"" + item->name + "\";\n";
		_cb += "\n";
		_cb += "    long lCount = 0;\n";
		_cb += "    //itemEntity.qxCount (lCount);\n";
		_cb += "    qDebug() << \"Count: \" << lCount << \".\";\n";
		_cb += "\n";
		create_contract_fields (
			item,
			item->subelements["fields"],
			[] (size_t i, std::string & str, sys::conf::block_t * item) {

				str += "    qDebug() << \"" + item->name + "\";\n";
				if (item->name == "Entity") {
					str += "    // Entity = __Disc__\n";
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    itemEntity.set__Disc__";
					str += " (QString::fromStdString(itemProto.";
					str += sys::lower(item->name);
					str += "()));\n";
					return;
				}

				if (item->values["type"] == "nvarchar") {
					logp (sys::e_debug, "nvarchar");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (\"" + item->name + "\");\n";
					str += "    QString str" + sys::itoa(++i)
						+ " = QString::fromStdString(itemProto."
						+ sys::lower(item->name) + "());\n";
					str += "    itemEntity.set" + item->name + " (str" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "integer") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    long integer" + sys::itoa(++i) + " = 10;\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (" + sys::itoa(i) + ");\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "integer" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "bit") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    int integer" + sys::itoa(++i) + " = 10;\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (" + sys::itoa(i) + ");\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "integer" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "datetime") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    QDateTime dt" + sys::itoa(++i) + ";\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "dt" + sys::itoa(i) + ");\n";
				} else if (item->values["type"] == "blob") {
					logp (sys::e_debug, "integer");
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    char * ch" + sys::itoa(++i) + " = \"abc\";\n";
					str += "    itemProto.set_" + sys::lower(item->name) + " (ch" + sys::itoa(i) + ");\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "ch" + sys::itoa(i) + ");\n";
				} else {
					logp (sys::e_debug, "other: " + item->values["type"]);
					str += "    // type '" + item->values["type"] + "'\n";
					str += "    long other" + sys::itoa(++i) + " = 10;\n";
					str += "    itemEntity.set" + item->name + " ("
						+ "other" + sys::itoa(i) + ");\n";
				}
			});
		_cb += "    qx::dao::update(itemEntity);\n";
		_cb += "    response = params;\n";
		_cb += "    return response;\n";
		_cb += "}\n";
		_cb += "\n";

		//
		// Delete
		//
		_cb += "\n";
		_cb += "QByteArray ";
		_cb += business;
		_cb += "::Delete (const QByteArray &params)\n";
		_cb += "{\n";
		_cb += "    qDebug() << \"GetAll\";\n";
		_cb += "    QByteArray response;\n";
		_cb += "    " + item->name + " itemEntity;\n";
		_cb += "    Bytech::Hulk::" + item->name + " itemProto;\n";		
		_cb += "    qDebug() << \"" + item->name + "\";\n";
		_cb += "\n";
		_cb += "    itemProto.ParseFromArray (params, params.size());\n";
		_cb += "    long lCount = 0;\n";
		_cb += "    //itemEntity.qxCount (lCount);\n";
		_cb += "    qDebug() << \"Count: \" << lCount << \".\";\n";
		_cb += "\n";
		_cb += "\n";
		_cb += "    itemEntity.setId(itemProto.id());\n";
		_cb += "    QSqlError error = itemEntity.qxDeleteById();\n";
		_cb += "    if (error.isValid()) {\n";
		_cb += "        qDebug() << \"Error: \" << error.text();\n";
		_cb += "        Bytech::Hulk::HulkResult result;\n";
		_cb += "        Bytech::Base::Result res;\n";
		_cb += "\n";
		_cb += "        res.set_result_code (1);\n";
		_cb += "        response.resize (result.ByteSize());\n";
		_cb += "        result.SerializeToArray (response.data(), result.ByteSize());\n";
		_cb += "    } else {\n";
		_cb += "        qDebug() << \"Delete Success.\";\n";
		_cb += "        Bytech::Hulk::HulkResult result;\n";
		_cb += "        Bytech::Base::Result res;\n";
		_cb += "        Bytech::Base::MessageHeader header;\n";
		_cb += "        ";
		_cb += "\n";
		_cb += "        res.set_result_code (0);\n";
		_cb += "        response.resize (result.ByteSize());\n";
		_cb += "        result.SerializeToArray (response.data(), result.ByteSize());\n";
		_cb += "    }";
		_cb += "\n";
		_cb += "    return response;\n";
		_cb += "}\n";
		_cb += "\n";

		//create_contract_relations (item->subelements["foreigns"]);

		if (true) { // This is just to have a scope
			//
			//  Place in "contracts"
			//
			logp (sys::e_debug, "Header: " << (_path + "/" + item->name + ".h"));
			std::string hname (_path + "/" + item->name + "/" + item->name + "Business.h");
			std::ofstream header;

			sys::stat_t stat_header (hname);

			if (stat_header) {
				sys::file_system::safe_mv (hname, hname + ".old");
			}

			header.open (hname);
			if (! header.is_open()) {
				logp (sys::e_crit, "NO HEADER FILE!!! " << hname);
				throw file ("Can't create header file", __FILE__, __LINE__);
			}
			header << _ch;


			std::string bname (_path + "/" + item->name + "/" + item->name + "Business.cpp");
			sys::stat_t stat_body (bname);

			if (stat_body) {
				sys::file_system::safe_mv (bname, bname + ".old");
			}

			std::ofstream body (bname);
			if (! body.is_open()) {
				logp (sys::e_crit, "NO BODY FILE!!! " << bname);
				throw file ("Can't create body file", __FILE__, __LINE__);
			}
			body << _cb;
		}

		if (true) { // This is just to have a scope
			//
			// Place in business dir
			//
			logp (sys::e_debug, "Header: /contracts/" << (item->name + ".h"));
			std::string hname (_path + "/contracts/" + item->name + "Business.h");
			std::ofstream header;

			sys::stat_t stat_header (hname);

			if (stat_header) {
				sys::file_system::safe_mv (hname, hname + ".old");
			}

			header.open (hname);
			if (! header.is_open()) {
				logp (sys::e_crit, "NO HEADER FILE!!! " << hname);
				throw file ("Can't create header file", __FILE__, __LINE__);
			}
			header << _ch;


			std::string bname (_path + "/contracts/" + item->name + "Business.cpp");
			sys::stat_t stat_body (bname);

			if (stat_body) {
				sys::file_system::safe_mv (bname, bname + ".old");
			}

			std::ofstream body (bname);
			if (! body.is_open()) {
				logp (sys::e_crit, "NO BODY FILE!!! " << bname);
				throw file ("Can't create body file", __FILE__, __LINE__);
			}
			body << _cb;
		}
	}

	template<typename Func>
	void generator::create_contract_fields (sys::conf::block_t * item,
											sys::conf::block_t * fields,
											Func func)
	{
		using namespace sys;
		flog ();

		conf::block_t::mapblock_t::iterator itb = fields->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = fields->subelements.end();

		logp (sys::e_debug, "\t\t" << item->name);

		for (int i = 0; itb != ite; ++itb, ++i) {
			sys::conf::block_t * rel = item->subelements["relations"];

			// If there are no relations on this field, test it
			if ((rel && !rel->subelements[itb->second->name]) || !rel) {

				if (! itb->second) {
					logp (sys::e_debug, "\t\tNo field!");
					break;
				}
				create_contract_field (i, itb->second, func);
			} else {
				if (! itb->second) {
					logp (sys::e_debug, "\t\tNo field!");
					break;
				}
				//create_contract_field (i, itb->second, func);
				create_contract_relations (itb->second);
			}
		}

	}

	template<typename Func>
	void generator::create_contract_field (size_t i,
										   sys::conf::block_t * item,
										   Func func)
	{
		using namespace sys;
		flog ();

		std::string type = item->values["type"];
		std::string primary = item->values["primary"];
		std::string not_null = item->values["not_null"];
		std::string def = item->values["default"];
		std::string size = item->values["size"];
		
		logp (sys::e_debug, "\t\t\tField: "
			  << item->name << ", "
			  << type << ", "
			  << primary << ", "
			  << not_null << ", "
			  << def << ", "
			  << size << ".");

		func (i, _cb, item);

		// QString::fromStdString(
		/*
		_cb += "(itemProto.";
		_cb += sys::lower(item->name);
		_cb += "());\n";
		*/

		if (item->values["type"] == "nvarchar") {
			logp (sys::e_debug, "nvarchar");
			_db += "    // type '" + item->values["type"] + "'\n";
			_db += "    QString str" + sys::itoa(++i)
				+ " QString::fromStdString(temProto."
				+ sys::lower(item->name) + ");\n";
			_db += "    itemEntity.set" + item->name;
			_db += " );\n";
		} else if (item->values["type"] == "integer") {
			logp (sys::e_debug, "integer");
			_db += "    // type '" + item->values["type"] + "'\n";
			_db += "    long integer" + sys::itoa(++i) + " = 10;\n";
			_db += "    elem.set" + item->name + "("
			   + "integer" + sys::itoa(i) + ");\n";
		} else if (item->values["type"] == "bit") {
			logp (sys::e_debug, "integer");
			_db += "    // type '" + item->values["type"] + "'\n";
			_db += "    int integer" + sys::itoa(++i) + " = 10;\n";
			_db += "    elem.set" + item->name + "("
			   + "integer" + sys::itoa(i) + ");\n";
		} else if (item->values["type"] == "datetime") {
			logp (sys::e_debug, "integer");
			_db += "    // type '" + item->values["type"] + "'\n";
			_db += "    QDateTime dt" + sys::itoa(++i) + ";\n";
			_db += "    elem.set" + item->name + "("
			   + "dt" + sys::itoa(i) + ");\n";
		} else if (item->values["type"] == "blob") {
			logp (sys::e_debug, "integer");
			_db += "    // type '" + item->values["type"] + "'\n";
			_db += "    char * ch" + sys::itoa(++i) + " = \"abc\";\n";
			_db += "    elem.set" + item->name + "("
			   + "ch" + sys::itoa(i) + ");\n";
		} else {
			logp (sys::e_debug, "other: " + item->values["type"]);
			_db += "    // type '" + item->values["type"] + "'\n";
			_db += "    long other" + sys::itoa(++i) + " = 10;\n";
			_db += "    elem.set" + item->name + "("
			   + "other" + sys::itoa(i) + ");\n";
		}


	}

	void generator::create_contract_relations (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();

		// There may be cases where there are no relations
		if (! item)
			return;

		conf::block_t::mapblock_t::iterator itb = item->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = item->subelements.end();

		for (; itb != ite; ++itb) {
			if (! itb->second)
				break;
			create_contract_relation (itb->second);
		}

	}

	void generator::create_contract_relation (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();

		std::string table = item->values["table"];
		std::string field = item->values["field"];
		
		logp (sys::e_debug, "Relation: "
			  << item->name << ", "
			  << table << ", "
			  << field << ".");
		_cb += "    // This field has relations\n";
	}

	//
	// Create angular modules, routes, controllers and data providers
	//
	void generator::create_angular (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool angular = item->values["gen_angular"] == "true";

		if (! angular) {
			logp (sys::e_debug, "NO ANGULAR!!!");
			logp (sys::e_debug, "'" << item->values["gen_angular"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());

		conf::block_t::mapblock_t::iterator itb = lib->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = lib->subelements.end();

		std::string tmp_path (_path);

		for (; itb != ite; ++itb) {
			logp (sys::e_debug, "Angular: " << itb->second->name);
			_path = tmp_path + "/" + itb->second->name;
			sys::file_system::mkdir (_path);

			create_angular_module (itb->second);
			create_angular_route (itb->second);
			create_angular_controller (itb->second);
			create_angular_data (itb->second);
		}

		_path = tmp_path;
		create_angular_net (item);
	}

	void generator::create_angular_module (sys::conf::block_t * item)
	{
		std::string name (item->name);
		std::string module_name (_path + "/module.js");
		std::ofstream module;
		sys::stat_t stat (module_name);

		logp (sys::e_debug, "AngularJS module '" << module_name << "'.");

		if (stat){
			sys::file_system::safe_mv (module_name, module_name + ".old");
		}

		module.open (module_name);
		if (! module.is_open()) {
			logp (sys::e_crit, "NO MODULE FILE!!!");
			throw file ("Can't create module (angular module) file", __FILE__, __LINE__);
		}
		module << "//\n";
		module << "//\n";
		module << "//\n";
		module << "//\n";
		module << "\n";
		module << "(function () {\n";
		module << "    'use strict';\n";
		module << "\tangular\n";
		module << "\t\t.module ('hulk.";
		module << name;
		module << "', ['hulk.data', 'hulk.net']);\n";
		module << "}) ();\n";
	}
	
	void generator::create_angular_route (sys::conf::block_t * item)
	{
		std::string name (item->name);
		std::string route_name (_path + "/route.js");
		std::ofstream route;
		sys::stat_t stat (route_name);

		logp (sys::e_debug, "AngularJS route '" << route_name << "'.");

		if (stat) {
			sys::file_system::safe_mv (route_name, route_name + ".old");
		}

		route.open (route_name);
		if (! route.is_open()) {
			logp (sys::e_crit, "NO ROUTE FILE!!!");
			throw file ("Can't create route (angular route) file", __FILE__, __LINE__);
		}
		route << "//\n";
		route << "//\n";
		route << "//\n";
		route << "//\n";
		route << "(function () {\n";
		route << "    'use strict';\n";
		route << "\n";
		route << "  angular\n";
		route << "	.module ('hulk.";
		route << item->name;
		route << "')\n";
		route << "	.config (['$routeProvider',";
		route << "	routes";
		route << name;
		route << "]);";
		route << "\n";
		route << "    function routes";
		route << name;
		route << " ($routeProvider) {\n";
		route << "	$routeProvider";

		route << "        .when (\n";
		route << "	    '/";
		route << name;
		route << "',\n";
		route << "	    { templateUrl : '";
		route << name;
		route << "/";
		route << name;
		route << ".html', ";
		route << " controller : '";
		route << name;
		route << "Ctl' ";
		route << "})\n";

		route << "        .when (\n";
		route << "	    '/";
		route << name;
		route << "/edit/:id',\n";
		route << "	    { templateUrl : '";
		route << name;
		route << "/edit.html',";
		route << " controller : '";
		route << name;
		route << "Ctl' ";
		route << "})\n";

		route << "        .when (\n";
		route << "	    '/";
		route << name;
		route << "/delete/:id',\n";
		route << "	    { templateUrl : '";
		route << name;
		route << "/delete.html', ";
		route << " controller : '";
		route << name;
		route << "Ctl' ";
		route << "});\n";

		route << "}\n}) ();\n";
	}

	void generator::create_angular_controller (sys::conf::block_t * item)
	{
		std::string name (item->name);
		std::string controller_name (_path + "/controller.js");
		std::ofstream controller;
		sys::stat_t stat (controller_name);

		logp (sys::e_debug, "AngularJS controller '" << controller_name << "'.");

		if (stat) {
			sys::file_system::safe_mv (controller_name, controller_name + ".old");
		}

		controller.open (controller_name);
		if (! controller.is_open()) {
			logp (sys::e_crit, "NO CONTROLLER FILE!!!");
			throw file ("Can't create controller (angular controller) file",
						__FILE__,
						__LINE__);
		}
		controller << "//\n";
		controller << "//\n";
		controller << "//\n";
		controller << "//\n";
		controller << "\n";

		controller << "(function() {\n";
		controller << "    'use strict';\n";
		controller << "\n";
		controller << "    angular\n";
		controller << "        .module ('hulk.";
		controller << name;
		controller << "')\n";
		controller << "        .controller (\n";
		controller << "	    '";
		controller << name;
		controller << "Ctl',\n";
		controller << "	    [\n";
		controller << "		'$scope',\n";
		controller << "		'log',\n";
		controller << "		";
		controller << name;
		controller << "Ctl\n";
		controller << "	    ]\n";
		controller << "	);\n";
		controller << "    \n";
		controller << "    function ";
		controller << name;
		controller << "Ctl ($scope, log) {\n";

		controller << "log.write ('En controller haciendo algo.');\n";

		sys::conf::block_t * fields = item->subelements["fields"];
		sys::conf::block_t::mapblock_t::iterator itb
			= fields->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator ite
			= fields->subelements.end();

		controller << "   $scope.item = {};\n";

		for (; itb != ite; ++itb) {
			sys::conf::block_t * field = itb->second;

			controller << "   $scope.item." << field->name << " = '';\n";
		}

		controller << "	\n";
		controller << "    }\n";
		controller << "\n";
		controller << "}) ();\n";

	}

	void generator::create_angular_net (sys::conf::block_t * item)
	{
		std::string net_name (_path + "/net.js");
		std::ofstream net;
		sys::stat_t stat (net_name);

		(void) item; // prevent warning

		if (stat) {
			sys::file_system::safe_mv (net_name, net_name + ".old");
		}

		logp (sys::e_debug, "AngularJS net '" << net_name << "'.");

		net.open (net_name);
		if (! net.is_open()) {
			logp (sys::e_crit, "NO NET FILE!!!");
			throw file ("Can't create net (angular net) file",
						__FILE__,
						__LINE__);
		}
		net << "//\n";
		net << "//\n";
		net << "//\n";
		net << "//\n";
		net << "(function () {\n";
		net << "'use strict';\n";
		net << "\n";
		net << "angular\n";
		net << "    .module ('hulk.net')\n";
		net << "    .factory ('network',\n";
		net << "              ['net', '$rootScope', '$q', networkFunc]);\n";
		net << "function networkFunc (net, rs, q) {\n";
		net << "    var callbacks = {};\n";
		net << "    var currentCallbackId = 0;\n";
		net << "    var ws = new WebSocket(ws://localhost:9090/');\n";
		net << "\n";
		net << "    function getCallbackId () {\n";
		net << "        currentCallbackId += 1;\n";
		net << "        if (currentCallbackId > 10000) {\n";
		net << "            currentCallbackId = 0;\n";
		net << "        }\n";
		net << "        return currentCallbackId;\n";
		net << "    }\n";

		net << "\n";

		net << "    ws.onopen = function (){  \n";
		net << "        console.log (\"Socket has been opened!\");  \n";
		net << "    };\n";
		net << "\n";
		net << "    ws.onmessage = function(message) {\n";
		net << "        listener (JSON.parse(message.data));\n";
		net << "    };\n";
		net << "\n";
		net << "    function sendRequest (request) {\n";
		net << "        var defer = $q.defer();\n";
		net << "        var callbackId = getCallbackId();\n";
		net << "        callbacks[callbackId] = {\n";
		net << "            time : new Date(),\n";
		net << "            cb : defer\n";
		net << "        };\n";
		net << "        request.callback_id = callbackId;\n";
		net << "        console.log ('Sending request.');\n";
		net << "        ws.send (request);\n";
		net << "        return defer.promise;\n";
		net << "    }\n";
		net << "\n";
		net << "    function listener (data) {\n";
		net << "        var messageObj = data;\n";
		net << "        console.log (\"Received data from websocket.\");\n";
		net << "        if (callbacks.hasOwnProperty(messageObj.callback_id)) {\n";
		net << "            console.log(callbacks[messageObj.callback_id]);\n";
		net << "            $rootScope.$apply (callbacks[messageObj.callback_id].cb.resolve(messageObj.data));\n";
		net << "            delete callbacks[messageObj.callbackID];\n";
		net << "        }\n";
		net << "    }\n";
		net << "\n";
		net << "    return {\n";
		net << "        send : function (obj) {\n";
		net << "            return sendRequest(obj)";
		net << "        }\n";
		net << "    };\n";
		net << "};\n";
		net << "}) ();\n";
	}

	void generator::create_angular_data (sys::conf::block_t * item)
	{
		std::string name (item->name);
		std::string data_name (_path + "/data.js");
		std::ofstream data;

		sys::stat_t stat (data_name);

		if (stat) {
			sys::file_system::safe_mv (data_name, data_name + ".old");
		}

		logp (sys::e_debug, "AngularJS data '" << data_name << "'.");

		data.open (data_name);
		if (! data.is_open()) {
			logp (sys::e_crit, "NO DATA FILE!!!");
			throw file ("Can't create data (angular data) file",
						__FILE__,
						__LINE__);
		}
		data << "//\n";
		data << "//\n";
		data << "//\n";
		data << "//\n";
		data << "(function () {\n";
		data << "'use strict';\n";
		data << "\n";
		data << "angular\n";
		data << ".module ('hulk.data')\n";
		data << ".factory ('";
		data << item->name;
		data << "',\n";
		data << "['net',\n";
		data << item->name;
		data << "Func]);\n";

		data << "function ";
		data << item->name;
		data << "Func (net) {\n";

		data << "    return {\n";
		data << "        get_all : function () {\n";
		data << "        return net.get_all()";
		data << "    },\n";
		data << "    getById : function (id) {\n";
		data << "        treturn net.get(id);";
		data << "    },\n";
		data << "        update : function (prod) {\n";
		data << "        prod.op = 'u';\n";
		data << "        return net.send(prod);\n";
		data << "    },\n";
		data << "        delete : function (prod) {\n";
		data << "        prod.op = 'd';\n";
		data << "        return net.send(prod);\n";
		data << "    }\n";
		data << "};\n";
		data << "};\n";
		data << "}) ();\n";

	}

	//
	// Create html test pages
	//
	void generator::create_html (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool data = item->values["gen_html"] == "true";

		if (! data) {
			logp (sys::e_debug, "NO HTML!!!");
			logp (sys::e_debug, "'" << item->values["gen_html"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());

		_ix = "<!doctype html>\n";
		_ix += "<html ng-app=\"hulk\">\n";
		_ix += "<body>\n";
		_ix += "<h2>Hulk main test page</h2>\n";

		std::string tmp_path (_path);
		std::string js_fname (tmp_path + "/main.js");
		std::ofstream js;

		sys::stat_t stat_js (js_fname);

		if (stat_js) {
			sys::file_system::safe_mv (js_fname, js_fname + ".old");
		}

		js.open (js_fname);
		if (! js.is_open()) {
			logp (sys::e_crit, "NO JS FILE!!!");
			throw file ("Can't create js file", __FILE__, __LINE__);
		}

		js << "(function () {\n";
		js << "'use strict';\n";
		js << "\n";
		js << "angular\n";
		js << ".module('hulk',\n";
		js << "['ngCookies',\n";
		js << "'ngRoute',\n";



		conf::block_t::mapblock_t::iterator itb = lib->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = lib->subelements.end();

		for (; itb != ite; ++itb) {
			logp (sys::e_debug, "Html: " << itb->second->name);
			_path = tmp_path + "/" + itb->second->name;
			sys::file_system::mkdir (_path);
			_ix += "<a ng-href=\"#/";
			_ix += itb->second->name;
			_ix += "\">";
			_ix += itb->second->name;
			_ix += "</a>, ";

			js << "'hulk."  << itb->second->name << "',\n";

			create_html_main (itb->second);
			create_html_edit (itb->second);
		}

		js << "'hulk.data'\n";
		js << "]);\n";
		js << "}) ();\n";
		_ix += "<hr />\n";
		_ix += "<h2>Página</h2>\n";
		_ix += "<div ng-view></div>\n";
		_ix += "<hr />\n";

		_ix += "<script src=\"angular.js\"></script>\n";
		_ix += "<script src=\"angular-cookies.js\"></script>\n";
		_ix += "<script src=\"angular-route.js\"></script>\n";
		_ix += "<script src=\"main.js\"></script>\n";
		_ix += "<script src=\"data.js\"></script>\n";
		_ix += "<script src=\"log.js\"></script>\n";
		_ix += "<script src=\"net.js\"></script>\n";

		for (itb = lib->subelements.begin(); itb != ite; ++itb) {
			// module
			_ix += "<script src=\"";
			_ix += itb->second->name;
			_ix += "/module.js\"></script>\n";
			// route
			_ix += "<script src=\"";
			_ix += itb->second->name;
			_ix += "/route.js\"></script>\n";
			// controller
			_ix += "<script src=\"";
			_ix += itb->second->name;
			_ix += "/controller.js\"></script>\n";
			// data
			_ix += "<script src=\"";
			_ix += itb->second->name;
			_ix += "/data.js\"></script>\n";
		}

		_ix += "</body>\n";
		_ix += "</html>\n";

		// Html buffer
		std::string html_fname (tmp_path + "/index.html");
		std::ofstream html;

		logp (sys::e_debug, "Create html file: '"
			  << html_fname
			  << "'.");

		sys::stat_t stat_html (html_fname);

		if (stat_html) {
			sys::file_system::safe_mv (html_fname, html_fname + ".old");
		}

		html.open (html_fname);
		if (! html.is_open()) {
			logp (sys::e_crit, "NO HTML FILE!!!");
			throw file ("Can't create html file", __FILE__, __LINE__);
		}
		html << _ix;
		// Html buffer end

		_path = tmp_path;
	}

	void generator::create_html_main (sys::conf::block_t * item)
	{
		flog ();
		std::string name (item->name);
		std::string data_name (_path + "/" + name + ".html");
		std::ofstream data;

		sys::stat_t stat (data_name);

		if (stat) {
			sys::file_system::safe_mv (data_name, data_name + ".old");
		}

		logp (sys::e_debug, "Html data '" << data_name << "'.");

		sys::stat_t stat_data (data_name);

		if (stat_data) {
			sys::file_system::safe_mv (data_name, data_name + ".old");
		}

		data.open (data_name);
		if (! data.is_open()) {
			logp (sys::e_crit, "NO HTML FILE!!!");
			throw file ("Can't create html (main) file",
						__FILE__,
						__LINE__);
		}

		sys::conf::block_t * fields = item->subelements["fields"];
		sys::conf::block_t::mapblock_t::iterator itb
			= fields->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator ite
			= fields->subelements.end();

		data << "<h3>" << name << "</h3>\n";

		for (; itb != ite; ++itb) {
			sys::conf::block_t * field = itb->second;

			data << "<td>" << field->name << "</td>\n";
			data << "<td>";
			data << "<input type=\"text\" ng-model=\""
				 << field->name << "\" />\n";
			data << field->values[""];
			data << "</td>\n";
			data << "</tr>\n";

		}
		data << "<button ng-click=\"edit()\">"
			 << "Edit</button>\n";
		data << "<button ng-click=\"delete()\">"
			 << "Delete</button>\n";
	}

	void generator::create_html_edit (sys::conf::block_t * item)
	{
		flog ();
		std::string name (item->name);
		std::string data_name (_path + "/" + name + ".edit.html");
		std::ofstream data;

		sys::stat_t stat (data_name);

		if (stat) {
			sys::file_system::safe_mv (data_name, data_name + ".old");
		}

		logp (sys::e_debug, "Html data '" << data_name << "'.");

		data.open (data_name);
		if (! data.is_open()) {
			logp (sys::e_crit, "NO HTML FILE!!!");
			throw file ("Can't create html (edit) file",
						__FILE__,
						__LINE__);
		}

		sys::conf::block_t * fields = item->subelements["fields"];
		sys::conf::block_t::mapblock_t::iterator itb
			= fields->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator ite
			= fields->subelements.end();

		data << "<h3>" << name << "</h3>\n";
		data << "<table>\n";


		for (; itb != ite; ++itb) {
			sys::conf::block_t * field = itb->second;

			data << "<tr>\n";
			data << "<td>" << field->name << "</td>\n";
			data << "<td>";
			data << "<input type=\"text\" ng-model=\""
				 << field->name << "\" />\n";
			//data << field->values[""];
			data << "</td>\n";
			data << "</tr>\n";

		}

		data << "</table>\n";
		data << "<br />\n";
		data << "<button ng-click=\"update(item)\">"
			 << "Actualizar</button>\n";
	}

	//
	// Database tests
	//
	void generator::create_db_tests (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool dodb = item->values["gen_db"] == "true";
		std::string tests_path = item->values["tests_dir"];

		if (! dodb) {
			logp (sys::e_debug, "NO DB!!!");
			logp (sys::e_debug, "'" << item->values["gen_db"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);
		_conf_path = gene;
		conf::block_t * lib = _conf.find(gene.c_str());

		conf::block_t::mapblock_t::iterator itb = lib->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = lib->subelements.end();

		for (; itb != ite; ++itb) {
			logp (sys::e_debug, "Db: " << itb->second->name);
			itb->second->values["tests_dir"] = item->values["tests_dir"];
			create_db_main (itb->second);
		}
	}

	void generator::create_db_main (sys::conf::block_t * item)
	{
		std::ofstream db;
		std::string test_path = item->values["tests_dir"];

		sys::file_system::mkdir (_path + "/" + test_path);

		db.open (_path + "/" + test_path + "/" + item->name + ".db.cpp");
		if (! db.is_open()) {
			logp (sys::e_crit, "NO DB FILE!!!");
			throw file ("Can't create db file", __FILE__, __LINE__);
		}

		db << "#include <QxOrm.h>\n";
		db << "#include \"hulk_all_include.gen.h\"\n";
		db << "\n";
		db << "\n";
		db << "\n";
		db << "int main (int argc, char ** argv)\n";
		db << "{\n";
		db << "\n";
		db << "    QCoreApplication a(argc, argv);\n";
		db << "\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setDriverName(\"QSQLITE\");\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setDatabaseName(\"./hulk.db\");\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setHostName(\"localhost\");\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setUserName(\"root\");\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setPassword(\"\");\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(true);\n";
		db << "    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(true);\n";
		db << "\n";

		create_db_details (db, item);

		db << "    }\n";
		db << "    return 0;\n";
		db << "}";
	}

	void generator::create_db_details (std::ofstream & db,
									   sys::conf::block_t * item)
	{
		flog ();

		sys::conf::block_t * rel = item->subelements["relations"];

		// If there are relations on this table, generate
		// required things
		if (rel) {
			create_db_relations (db, rel);
		}

		db << "    " << item->name << " var" << item->name << ";\n";
		db << "\n";

		sys::conf::block_t * fields = item->subelements["fields"];
		sys::conf::block_t::mapblock_t::iterator fitb
			= fields->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator fite
			= fields->subelements.end();

		for (int i = 0; fitb != fite; ++fitb, ++i) {
			sys::conf::block_t * field = fitb->second;

			nlogp (sys::e_debug, "Look for relation in '"
				   << itb->second->name
				   << "' for '"
				   << fitb->second->name
				   << "'.");
			// If there are no relations on this field, test it
			//if ((rel && !rel->subelements[fitb->second->name]) || !rel) {

			logp (sys::e_debug, "Element: "
				  << fitb->second->name);
					  
			if (field->values["type"] == "nvarchar") {
				logp (sys::e_debug, "nvarchar");
				db << "    // type '" << field->values["type"] << "'\n";
				db << "    elem.set" << field->name << "(";
				db << "\"aaaaaaaa\");\n";
			} else if (field->values["type"] == "integer") {
				logp (sys::e_debug, "integer");
				db << "    // type '" << field->values["type"] << "'\n";
				db << "    long integer" << sys::itoa(++i) << " = 10;\n";
				db << "    elem.set" << field->name << "("
				   << "integer" << sys::itoa(i) << ");\n";
			} else if (field->values["type"] == "bit") {
				logp (sys::e_debug, "integer");
				db << "    // type '" << field->values["type"] << "'\n";
				db << "    int integer" << sys::itoa(++i) << " = 10;\n";
				db << "    elem.set" << field->name << "("
				   << "integer" << sys::itoa(i) << ");\n";
			} else if (field->values["type"] == "datetime") {
				logp (sys::e_debug, "integer");
				db << "    // type '" << field->values["type"] << "'\n";
				db << "    QDateTime dt" << sys::itoa(++i) << ";\n";
				db << "    elem.set" << field->name << "("
				   << "dt" << sys::itoa(i) << ");\n";
			} else if (field->values["type"] == "blob") {
				logp (sys::e_debug, "integer");
				db << "    // type '" << field->values["type"] << "'\n";
				db << "    char * ch" << sys::itoa(++i) << " = \"abc\";\n";
				db << "    elem.set" << field->name << "("
				   << "ch" << sys::itoa(i) << ");\n";
			} else {
				logp (sys::e_debug, "other: " << field->values["type"]);
				db << "    // type '" << field->values["type"] << "'\n";
				db << "    long other" << sys::itoa(++i) << " = 10;\n";
				db << "    elem.set" << field->name << "("
				   << "other" << sys::itoa(i) << ");\n";
			}
			//} else {
			//logp (sys::e_debug, "Hmm... this one has a relation, sure.");
			//}
		}

		db << "    elem.qxInsert();\n";
		db << "    list_of_" << item->name << " lst;\n";
		db << "    elem.qxFetchAll(lst);\n";
		db << "    foreach(auto l, lst)\n";
		db << "    {\n";

		fitb = fields->subelements.begin();
		for (; fitb != fite; ++fitb) {
			sys::conf::block_t * field = fitb->second;
			sys::conf::block_t * rel = item->subelements["relations"];

			if ((rel && !rel->subelements[fitb->second->name]) || !rel) {
				db << "      qDebug() << l.second->get";
				db << field->name;
				db << "();\n";
			}
		}
	}

	void generator::create_db_relations (std::ofstream & db,
										 sys::conf::block_t * item)
	{
		flog ();
		sys::conf::block_t::mapblock_t::iterator fitb
			= item->subelements.begin ();
		sys::conf::block_t::mapblock_t::iterator fite
			= item->subelements.end ();

		db << "\t// There ARE relations...\n";
		db << "\t//\n";
		db << "\t// " << item->name << "\n";
		db << "\t//\n";

		for (; fitb != fite; ++fitb) {
			sys::conf::block_t * elem = fitb->second;

			if (elem) {
				std::string name = elem->values["table"];
				db << "\t// the relation class name is... " << name << "\n";
				sys::conf::block_t * table = _conf.find((_conf_path + "/" + name).c_str());

				(void) table; // prevent warning
				//create_db_details (db, table);

			} else {
				db << "\t// Jajaja noooo pasaaaa naaa, pero no funciona\n";
			}
		}

	}

	//
	// Configure.ac
	//
	void generator::create_configure_ac (sys::conf::block_t * item)
	{
		using namespace sys;
		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());

		std::string tmp_path (_path);

		sys::file_system::mkdir (tmp_path);

		std::ofstream conf;
		conf.open (tmp_path + "/configure.ac");
		if (! conf.is_open()) {
			logp (sys::e_crit, "NO CONFIGURE.AC FILE!!!");
			throw file ("Can't create configure.ac file", __FILE__, __LINE__);
		}

		conf << "AC_PREREQ(2.53)\n";
		conf << "AC_INIT([hulk], [0.1.0], [manuelcano@by.com.es])\n";
		conf << "AC_CONFIG_AUX_DIR([build-aux])\n";
		conf << "AC_CONFIG_MACRO_DIR([m4])\n";
		conf << "AM_INIT_AUTOMAKE([subdir-objects dist-bzip2])\n";
		conf << "AC_CONFIG_SRCDIR([Hulk.pb.cc])\n";

		conf << "AC_PROG_CXX\n";
		conf << "AC_PROG_CC\n";
		conf << "AC_PROG_LIBTOOL\n";

		conf << "\n";
		conf << "AC_SUBST(CFLAGS)\n";
		conf << "AC_SUBST(CXXFLAGS)\n";
		conf << "AC_CONFIG_FILES([ Makefile\n";

		std::string makes;
		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();
		for (; itb != ite; ++itb) {
			makes += "\t" + itb->second->name + "/Makefile \\\n";
		}
		conf << makes;
		conf << "\ttests/Makefile])\n";
		conf << "AC_CONFIG_TESTDIR([tests])\n";
		conf << "AC_OUTPUT\n";
		conf << "\n";
	}

	//
	// Makefile.am
	//
	void generator::create_makefile_am (sys::conf::block_t * item)
	{
		using namespace sys;
		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());

		std::string tmp_path (_path);

		sys::file_system::mkdir (tmp_path);

		std::ofstream make;
		make.open (tmp_path + "/Makefile.am");
		if (! make.is_open()) {
			logp (sys::e_crit, "NO MAKEFILE.AM FILE!!!");
			throw file ("Can't create Makefile.am file", __FILE__, __LINE__);
		}

		std::string makes;
		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();
		make << "SUBDIRS = \\\n";

		for (; itb != ite; ++itb) {
			makes += "\t" + itb->second->name + " \\\n";
		}
		// remove trailing newline, backslash and space
		//makes = makes.substr(0, makes.size() - 3);
		make << makes;
		make << "\ttests\n";
		make << "\n";
	}

	//
	// Testsuite.at
	//
	void generator::create_makefile (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool dodb = item->values["gen_db"] == "true";

		if (! dodb) {
			logp (sys::e_debug, "NO DB!!!");
			logp (sys::e_debug, "'" << item->values["gen_db"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);
		std::string test_path = item->values["tests_dir"];

		conf::block_t * lib = _conf.find(gene.c_str());

		std::string tmp_path (_path + "/" + test_path);

		sys::file_system::mkdir (tmp_path);

		std::ofstream test;
		logp (sys::e_debug, "Test suite file: " << test_path);
		test.open (tmp_path + "/testsuite.at");
		if (! test.is_open()) {
			logp (sys::e_crit, "NO TEST SUITE FILE!!!");
			throw file ("Can't create test suite file", __FILE__, __LINE__);
		}

		test << "AT_INIT([Hulk project test suite.])\n";
		test << "AT_COPYRIGHT([Copyright (c) 2015 By Tech])\n";
		test << "AT_COLOR_TESTS\n";
		test << "\n";
		test << "m4_include([basic.at])\n";

		std::ofstream make;
		logp (sys::e_debug, "Makefile file: " << test_path);
		make.open (tmp_path + "/Makefile.am");
		if (! make.is_open()) {
			logp (sys::e_crit, "NO DB FILE!!! " << test_path);
			throw file ("Can't create db file", __FILE__, __LINE__);
		}

		logp (sys::e_debug, "Begin work: " << test_path);

		make << "#\n";
		make << "#\n";
		make << "#\n";

		make << "# The `:;' works around a Bash 3.2 bug when the output is not writeable.\n";
		make << "$(srcdir)/package.m4: $(top_srcdir)/configure.ac \n";
		make << "	:;{ \\\n";
		make << "	  echo '# Signature of the current package.' &&	 \\\n";
		make << "	  echo 'm4_define([AT_PACKAGE_NAME],  [$(PACKAGE_NAME)])' && \\\n";
		make << "	  echo 'm4_define([AT_PACKAGE_TARNAME],  [$(PACKAGE_TARNAME)])' && \\\n";
		make << "	  echo 'm4_define([AT_PACKAGE_VERSION],  [$(PACKAGE_VERSION)])' && \\\n";
		make << "	  echo 'm4_define([AT_PACKAGE_STRING],  [$(PACKAGE_STRING)])' && \\\n";
		make << "	  echo 'm4_define([AT_PACKAGE_BUGREPORT],  [$(PACKAGE_BUGREPORT)])'; \\\n";
		make << "	  echo 'm4_define([AT_PACKAGE_URL],  [$(PACKAGE_URL)])'; \\\n";
		make << "	} > $@-t\n";
		make << "	mv $@-t $@\n";

		make << "\n";

		make << "AM_CPPFLAGS =  \\\n";
		make << "	 -Wall -W -D_REENTRANT -fPIE \\\n";
		make << "	-D_QX_ENABLE_BOOST_SERIALIZATION \\\n";
		make << "	-D_QX_ENABLE_BOOST_SERIALIZATION_BINARY \\\n";
		make << "	-D_QX_ENABLE_BOOST_SERIALIZATION_XML \\\n";
		make << "	-D_QX_MODE_DEBUG -DQT_SQL_LIB -DQT_CORE_LIB \\\n";
		make << "	-I$(top_srcdir) \\\n";
		make << "	-I$(top_srcdir)/include \\\n";
		make << "	-I/home/manuel/by/hulk/libs/boost/include \\\n";
		make << "	-I/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/include \\\n";
		make << "	-I/home/manuel/by/hulk/exteral/QxOrm/include \\\n";
		make << "	-I/home/manuel/Qt/5.3/gcc_64/include\n";
		make << "\n";
		make << "AM_LDFLAGS = \\\n";
		make << "	-Wl,-rpath,/home/manuel/Qt/5.3/gcc_64 -Wl,-rpath,/home/manuel/Qt/5.3/gcc_64/lib \\\n";
		make << "	-Wl,-rpath,/home/manuel/build-QxOrm-Desktop_Qt_5_3_GCC_64bit-Debug/lib \\\n";
		make << "	-Wl,-rpath,/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/bin \\\n";
		make << "	-L/home/manuel/QxOrm/include \\\n";
		make << "	-L/home/manuel/by/hulk/bytech-hulk/output/pc/debug \\\n";
		make << "	-L/home/manuel/Qt/5.3/gcc_64/lib\n";
		make << "\n";
		make << "LIBS = -lboost_serialization  -lQxOrmd -lbytech-hulk-entitiesd  -lQt5Sql -lQt5Core -lpthread\n";
		make << "\n";

		//conf::block_t::mapblock_t::iterator itb =
		//lib->subelements.begin();
		//conf::block_t::mapblock_t::iterator ite =
		//lib->subelements.end();
		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();
		make << "check_PROGRAMS = ";

		logp (sys::e_debug, "Begin programs: " << test_path);

		for (; itb != ite; ++itb) {
			make << itb->second->name << " ";
		}
		make << "\n";
		make << "\n";

		logp (sys::e_debug, "Begin sources: " << test_path);

		itb = lib->subelements.begin();
		for (; itb != ite; ++itb) {
			make << itb->second->name << "_SOURCES = " << itb->second->name << ".cc\n";
		}
		make << "\n";
		make << "\n";

		make << "EXTRA_DIST = testsuite.at basic.at package.m4 $(TESTSUITE)\n";
		make << "TESTSUITE = $(srcdir)/testsuite\n";
		make << "TESTS_ENVIRONMENT = top_builddir=$(top_builddir)\n";


		make << "check-local: atconfig $(TESTSUITE)\n";
		make << "\t$(SHELL) '$(TESTSUITE)' $(TESTSUITEFLAGS)\n";
		make << "\n";
		make << "installcheck-local: atconfig $(TESTSUITE)\n";
		make << "\t$(SHELL) '$(TESTSUITE)' AUTOTEST_PATH='$(bindir)' \\\n";
		make << "\t$(TESTSUITEFLAGS)\n";
		make << "\n";
		make << "clean-local:\n";
		make << "\ttest ! -f '$(TESTSUITE)' ||	$(SHELL) '$(TESTSUITE)' --clean\n";
		make << "\n";
		make << "AUTOM4TE = autom4te\n";
		make << "AUTOTEST = $(AUTOM4TE) --language=autotest\n";
		make << "$(TESTSUITE): $(srcdir)/testsuite.at $(srcdir)/basic.at $(srcdir)/package.m4\n";
		make << "\t$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $@.at\n";
		make << "\tmv $@.tmp $@\n";
		make << "\n";

		logp (sys::e_debug, "Begin basic.at: " << test_path);

		std::ofstream basic;
		logp (sys::e_debug, "basic.at file: " << test_path);
		basic.open (tmp_path + "/basic.at");
		if (! basic.is_open()) {
			logp (sys::e_crit, "NO basic.at FILE!!!");
			throw file ("Can't create basic.at file", __FILE__, __LINE__);
		}

		itb = lib->subelements.begin();
		for (; itb != ite; ++itb) {
			nlogp (sys::e_debug, "Db: " << itb->second->name);

			basic << "dnl ----------------------------------------------------------\n";
			basic << "dnl " << itb->second->name << "\n";
			basic << "dnl ----------------------------------------------------------\n";
			basic << "AT_SETUP([" << itb->second->name << "])\n";
			basic << "AT_KEYWORDS([db])\n";
			basic << "AT_CHECK([" << itb->second->name << "],[0],[ignore],[ignore])\n";
			basic << "AT_CLEANUP\n";
			basic << "\n";
		}
	}

	void generator::create_qtsql (sys::conf::block_t * item)
	{
		flog ();
		(void) item; // prevent warning
	}

	void generator::create_sub_makefile_am (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool submake = item->values["gen_submake"] == "true";


		if (! submake) {
			logp (sys::e_debug, "NO Sub Makefile.am!!!");
			logp (sys::e_debug, "'" << item->values["gen_submake"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());

		std::string tmp_path (_path);

		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();

		for (; itb != ite; ++itb) {
			std::string fname (tmp_path
							   + "/"
							   + itb->second->name
							   + "/Makefile.am");
			sys::file_system::mkdir (tmp_path + "/" + itb->second->name);
			std::ofstream make;
			logp (sys::e_debug, "Makefile.am file: " << fname);
			make.open (fname);
			if (! make.is_open()) {
				logp (sys::e_crit, "NO Makefile.am FILE!!!");
				throw file ("Can't create .pro file", __FILE__, __LINE__);
			}


			make << "AM_CPPFLAGS =  \\\n";
			make << "	 -Wall -W -D_REENTRANT -fPIE \\\n";
			make << "	-D_QX_ENABLE_BOOST_SERIALIZATION \\\n";
			make << "	-D_QX_ENABLE_BOOST_SERIALIZATION_BINARY \\\n";
			make << "	-D_QX_ENABLE_BOOST_SERIALIZATION_XML \\\n";
			make << "	-D_QX_MODE_DEBUG -DQT_SQL_LIB -DQT_CORE_LIB \\\n";
			make << "	-I$(top_srcdir) \\\n";
			make << "	-I$(top_srcdir)/include \\\n";
			make << "	-I/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/include \\\n";
			make << "	-I/home/manuel/by/hulk/bytech-hulk/bytech-hulk-wscommunications \\\n";
			make << "	-I/home/manuel/QxOrm/include \\\n";
			make << "   -I/home/manuel/Qt/5.3/gcc_64/include/QtCore \\\n";
			make << "   -I/home/manuel/Qt/5.3/gcc_64/include/QtWebSockets \\\n";
			make << "	-I/home/manuel/Qt/5.3/gcc_64/include\n";
			make << "\n";
			make << "AM_LDFLAGS = \\\n";
			make << "	-Wl,-rpath,/home/manuel/Qt/5.3/gcc_64 -Wl,-rpath,/home/manuel/Qt/5.3/gcc_64/lib \\\n";
			make << "	-Wl,-rpath,/home/manuel/build-QxOrm-Desktop_Qt_5_3_GCC_64bit-Debug/lib \\\n";
			make << "	-Wl,-rpath,/home/manuel/by/hulk/bytech-hulk/output/pc/debug \\\n";
			make << "	-L/home/manuel/QxOrm/include \\\n";
			make << "	-L/home/manuel/by/hulk/bytech-hulk/output/pc/debug \\\n";
			make << "	-L/home/manuel/Qt/5.3/gcc_64/lib\n";
/*
			make << "AM_CPPFLAGS = -DG_LOG_DOMAIN=\"sqler\"	\\\n";
			make << "	 -Wall -W -D_REENTRANT -fPIE	\\\n";
			make << "	-D_QX_ENABLE_BOOST_SERIALIZATION	\\\n";
			make << "	-D_QX_ENABLE_BOOST_SERIALIZATION_BINARY \\\n";
			make << "	-D_QX_ENABLE_BOOST_SERIALIZATION_XML	\\\n";
			make << "	-D_QX_MODE_DEBUG -DQT_SQL_LIB -DQT_CORE_LIB \\\n";
			make << "	-I$(top_srcdir)	\\\n";
			make << "	-I$(top_srcdir)/include \\\n";
			make << "	-I/home/manuel/Qt/5.3/gcc_64/include \\\n";
			make << "	-I/home/manuel/Qt/5.3/gcc_64/include/QtCore \\\n";
			make << "   -I../../../libs/boost/include \\\n";
			make << "   -I../../../libs/qxorm/include \\\n";
			make << "   -I../../bytech-hulk-wscommunications \n";

			make << "\n";
			make << "AM_LDFLAGS = \\\n";
			make << "	-Wl,-rpath,/home/manuel/Qt/5.3/gcc_64 -Wl,-rpath,/home/manuel/Qt/5.3/gcc_64/lib \\\n";
			make << "	-Wl,-rpath,/home/manuel/by/hulk/libs/boost/lib \\\n";
			make << "	-Wl,-rpath,/home/manuel/by/hulk/libs/qxorm/lib \\\n";
			make << "	-Wl,-rpath,/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/bin \\\n";
			make << "	-L/home/manuel/Qt/5.3/gcc_64/lib \\\n";
			make << "   -L../../libs/boost/lib -lboost_serialization \\\n";
			make << "   -L../../libs/qxorm/lib -lQxOrmd \\\n";
			make << "   -L../protobuf/src -lprotobuf -lprotoc \\\n";
			make << "   -L../../bytech-hulk/bytech-hulk-entities/bin -lHulkd \n";
*/
			make << "\n";
			make << "LIBS = -lQxOrmd -lHulkd  -lQt5Sql -lQt5Core -lQt5WebSockets -lprotobuf -lpthread \\\n";
			make << "\t-lbytech-hulk-wscommunicationsd -lbytech-hulk-entitiesd -lbytech-hulk-protod \\\n";
			make << "\t -lboost_serialization\n";
			make << "#-licui18n -licuuc -licudata -lQt5Gui\n";

			make << "\n";
			make << "AM_CXXFLAGS = -std=gnu++11\n";
			make << "bin_PROGRAMS = au" << itb->second->name << "\n";
			make << "au" << itb->second->name << "_SOURCES = \\\n";
			make << "\t../entityprocessbase.h \\\n";
			make << "\t../requestparams.h \\\n";
			make << "\t../ProtoBase \\\n";
			make << "\t" << itb->second->name << "Business.h " << itb->second->name << "Business.cpp \\\n";
			make << "\t" << itb->second->name << ".incomingprocessor.h "
				 << itb->second->name << ".incomingprocessor.cpp \\\n";
			make << "\t" << itb->second->name << ".srv.h " << itb->second->name << ".srv.cpp \\\n";
			make << "\t../Hulk.pb.h ../Hulk.pb.cc \\\n";
			make << "\t../base.pb.h ../base.pb.cc \\\n";
			make << "\t../base.enums.pb.h ../base.enums.pb.cc \\\n";
			make << "\t../protohelper.h ../protohelper.cpp \\\n";
			make << "\t../protobuf.h ../protobuf.cpp \\\n";
			make << "\t../bymodule.h ../bymodule.cpp \\\n";
			make << "\t../../bytech-hulk-entities/include/" << itb->second->name << ".gen.h ";
			make << "\t../../bytech-hulk-entities/src/" << itb->second->name << ".gen.cpp \n";
		}
	}

	void generator::create_qtpro (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool doqtpro = item->values["gen_qtpro"] == "true";


		if (! doqtpro) {
			logp (sys::e_debug, "NO .PRO!!!");
			logp (sys::e_debug, "'" << item->values["gen_qtpro"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);
		std::string test_path = item->values["tests_dir"];

		conf::block_t * lib = _conf.find(gene.c_str());

		std::string tmp_path (_path);

		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();

		for (; itb != ite; ++itb) {
			std::string fname (tmp_path
							   + "/"
							   + itb->second->name
							   + "/"
							   + itb->second->name
							   + ".pro");
			sys::file_system::mkdir (tmp_path + "/" + itb->second->name);
			std::ofstream pro;
			logp (sys::e_debug, ".pro file: " << fname);
			pro.open (fname);
			if (! pro.is_open()) {
				logp (sys::e_crit, "NO .pro FILE!!!");
				throw file ("Can't create .pro file", __FILE__, __LINE__);
			}



			pro << "#-------------------------------------------------\n";
			pro << "#\n";
			pro << "# Project created by ByTech \n";
			pro << "#\n";
			pro << "#-------------------------------------------------\n";
			pro << "\n";
			pro << "#include($$PWD/../../Hulk.pri)\n";
			pro << "\n";
			pro << "QT -= gui\n";
			pro << "QT += gui websockets\n";
			pro << "TARGET = " << itb->second->name << "\n";
			pro << "CONFIG += console\n";
			pro << "CONFIG -= app_bundle\n";
			pro << "TEMPLATE = app\n";
			pro << "include($$PWD/../../testers-common.pri)\n";
			pro << "\n";
			pro << "CONFIG( debug, debug|release ) {\n";
			pro << "    unix:!macx: LIBS += -L$$BY_LIBOUTPUTDIR -lbytech-hulk-wscommunicationsd\n";
			pro << "} else {\n";
			pro << "    unix:!macx: LIBS += -L$$BY_LIBOUTPUTDIR -lbytech-hulk-wscommunications\n";
			pro << "}\n";
			pro << "\n";
			pro << "INCLUDEPATH += $$PWD/../../../libs/boost/include\n";
			pro << "DEPENDPATH += $$PWD/../../../libs/boost/include\n";
			pro << "LIBS += -L$$PWD/../../../libs/boost/lib -lboost_serialization\n";
			pro << "\n";
			pro << "INCLUDEPATH += $$PWD/../../../libs/qxorm/include\n";
			pro << "DEPENDPATH += $$PWD/../../../libs/qxorm/include\n";
			pro << "unix:!macx: LIBS += -L$$PWD/../../../libs/qxorm/lib -lQxOrmd\n";
			pro << "\n";
			pro << "LIBS += -L$$PWD/../../protobuf/src -lprotobuf -lprotoc\n";
			pro << "\n";
			pro << "LIBS += -L$$PWD/../../../bytech-hulk/bytech-hulk-entities/bin -lHulkd\n";
			pro << "message($$LIBS)\n";
			pro << "\n";
			pro << "\n";
			pro << "INCLUDEPATH += $$PWD/../../bytech-hulk-wscommunications\n";
			pro << "DEPENDPATH += $$PWD/../../output/pc/debug\n";
			pro << "DEPENDPATH += $$PWD/../output/pc/debug\n";
			pro << "INCLUDEPATH += $$PWD/../../include\n";
			pro << "INCLUDEPATH += $$PWD/../..\n";
			pro << "INCLUDEPATH += $$PWD/..\n";
			pro << "DEPENDPATH += $$PWD/../../bytech-hulk-wscommunications\n";
			pro << "\n";
			pro << "HEADERS += " << itb->second->name << "Business.h ";
			pro << itb->second->name << ".incomingprocessor.h ";
			pro << "../Hulk.pb.h ";
			pro << "../base.pb.h ";
			pro << "../base.enums.pb.h ";
			pro << "../entityprocessbase.h ";
			pro << "../protohelper.h ";
			pro << "../protobuf.h ";
			pro << "../requestparams.h ";
			pro << "../bymodule.h ";
			pro << "../ProtoBase ";
			pro << "../../bytech-hulk-entities/include/" << itb->second->name << ".gen.h\n";

			pro << "SOURCES += ";
			pro << itb->second->name << "Business.cpp ";
			pro << itb->second->name << ".srv.cpp ";
			pro << itb->second->name << ".incomingprocessor.cpp ";
			pro << "../Hulk.pb.cc ";
			pro << "../base.pb.cc ../base.enums.pb.cc ../protohelper.cpp ";
			pro << "../protobuf.cpp ../bymodule.cpp\n";

		}
	}

	void generator::create_servers (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());
		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();

 		for (int i = 1; itb != ite; ++itb, ++i) {
			create_indi_server (itb->second, i);
		}

	}

	void generator::create_indi_server (sys::conf::block_t * item, int entityNum)
	{
		sys::conf::block_t * fields = item->subelements["fields"];
		sys::conf::block_t::mapblock_t::iterator fitb
			= fields->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator fite
			= fields->subelements.end();

		std::string tmp_path (_path + "/" + item->name + "/");

		std::string fname (tmp_path + item->name + ".srv.cpp");
		std::string proc_header (tmp_path + item->name + ".incomingprocessor.h");
		std::string proc_body (tmp_path + item->name + ".incomingprocessor.cpp");

		std::ofstream srv;
		sys::stat_t stat_srv (fname);

		if (stat_srv) {
			sys::file_system::safe_mv (fname, fname + ".old");
		}

		srv.open (fname);
		if (! srv.is_open()) {
			logp (sys::e_crit, "NO Server.cpp FILE!!!");
			throw file ("Can't create Server.cpp file", __FILE__, __LINE__);
		}

		std::ofstream fproc_h;
		sys::stat_t stat_header (proc_header);

		if (stat_header) {
			sys::file_system::safe_mv (proc_header, proc_header + ".old");
		}

		fproc_h.open (proc_header);
		if (! fproc_h.is_open()) {
			logp (sys::e_crit, "NO incomingmessageprocessor.srv.cpp FILE!!!");
			throw file ("Can't create incomingmessageprocessor.cpp file", __FILE__, __LINE__);
		}

		std::ofstream fproc_b;
		sys::stat_t stat_body (proc_body);

		if (stat_body) {
			sys::file_system::safe_mv (proc_body, proc_body + ".old");
		}

		fproc_b.open (proc_body);
		if (! fproc_b.is_open()) {
			logp (sys::e_crit, "NO incomingmessageprocessor.srv.cpp FILE!!!");
			throw file ("Can't create incomingmessageprocessor.cpp file", __FILE__, __LINE__);
		}

		/* incoming message processor header */

		fproc_h << "//\n";
		fproc_h << "//\n";
		fproc_h << "//\n";
		fproc_h << "#ifndef INCOMINGMESSAGEPROCESSOR_H\n";
		fproc_h << "#define INCOMINGMESSAGEPROCESSOR_H\n";
		fproc_h << "\n";
		fproc_h << "#include <stdlib.h>\n";
		fproc_h << "#include <iostream>\n";
		fproc_h << "#include <QObject>\n";
		fproc_h << "#include <QWebSocket>\n";
		fproc_h << "#include <QDebug>\n";
		fproc_h << "#include <QByteArray>\n";
		fproc_h << "#include \"wsserver.h\"\n";
		fproc_h << "#include \"Hulk.pb.h\"\n";
		fproc_h << "#include \"base.pb.h\"\n";
		fproc_h << "#include \"base.enums.pb.h\"\n";
		fproc_h << "#include \"../../bytech-hulk-entities/include/hulk_all_include.gen.h\"\n";
		fproc_h << "#include \"entityprocessbase.h\"\n";
		fproc_h << "#include \"requestparams.h\"\n";
		fproc_h << "#include \"bymodule.h\"\n";
		fproc_h << "\n";
		fproc_h << "class IncomingMessageProcessor : public QObject\n";
		fproc_h << "{\n";
		fproc_h << "    Q_OBJECT\n";
		fproc_h << "public:\n";
		fproc_h << "    explicit IncomingMessageProcessor(QObject *parent = 0);\n";
		fproc_h << "    virtual ~IncomingMessageProcessor();\n";
		fproc_h << "\n";
		fproc_h << "public slots:\n";
		fproc_h << "    void processIncomingMessage(QWebSocket *socket, const QByteArray &message);\n";
		fproc_h << "signals:\n";
		fproc_h << "\n";
		fproc_h << "public slots:\n";
		fproc_h << "private:\n";
		fproc_h << "    Bytech::Hulk::BaseHeader header;\n";

		/* incoming message processor body */

		fproc_b << "//\n";
		fproc_b << "//\n";
		fproc_b << "//\n";
		fproc_b << "#include \"" << item->name << ".incomingprocessor.h\"\n";
		fproc_b << "\n";
		fproc_b << "IncomingMessageProcessor::IncomingMessageProcessor(QObject *parent) : QObject(parent)\n";
		fproc_b << "{\n";
		fproc_b << "\n";
		fproc_b << "}\n";
		fproc_b << "\n";
		fproc_b << "IncomingMessageProcessor::~IncomingMessageProcessor()\n";
		fproc_b << "{\n";
		fproc_b << "\n";
		fproc_b << "}\n";
		fproc_b << "\n";
		fproc_b << "void IncomingMessageProcessor::processIncomingMessage(QWebSocket* socket, const QByteArray &message)\n";
		fproc_b << "{\n";
		fproc_b << "    if (!socket)\n";
		fproc_b << "    {\n";
		fproc_b << "        qDebug() << \"Socket sender null pointer:\";\n";
		fproc_b << "        return;\n";
		fproc_b << "    }\n";
		fproc_b << "\n";
		fproc_b << "    header.ParseFromArray (message, message.size());\n";
		fproc_b << "\n";
		fproc_b << "    switch (header.header().entity()) {\n";


		/* standalone server */

		srv << "// -------------------------------------------------\n";
		srv << "// -------------------------------------------------\n";
		srv << "// -------------------------------------------------\n";
		srv << "#include <QObject>\n";
		srv << "#include <QWebSocket>\n";
		srv << "#include <QDebug>\n";
		srv << "#include <QByteArray>\n";
		srv << "#include <QxOrm.h>\n";
		srv << "#include <QCoreApplication>\n";
		srv << "#include <QString>\n";
		//srv << "#include \"../bytech-hulk-entities/include/hulk_all_include.gen.h\"\n";
		srv << "#include \"Hulk.pb.h\"\n";
		srv << "#include \"base.pb.h\"\n";
		srv << "#include \"base.enums.pb.h\"\n";
		srv << "#include \"entityprocessbase.h\"\n";
		srv << "#include \"requestparams.h\"\n";
		srv << "#include \"bymodule.h\"\n";
		srv << "#include \"wsserver.h\"\n";
		srv << "#include \"incomingmessageprocessor.h\"\n";
		srv << "\n";
		srv << "\n";
		srv << "int main (int argc, char ** argv)\n";
		srv << "{\n";
		srv << "    int port = 9090;\n";
		srv << "    if (argc > 1)\n";
		srv << "        port = atoi(argv[1]);\n";
		srv << "\n";
		srv << "    QCoreApplication a(argc, argv);\n";
		srv << "\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setDriverName(\"QSQLITE\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setDatabaseName(\"./hulk.db\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setHostName(\"localhost\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setUserName(\"root\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setPassword(\"\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(true);\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(true);\n";
		srv << "\n";
		srv << "\n";
		srv << "    IncomingMessageProcessor processor(&a);\n";
		srv << "    QString certFile(\"../ssl-certificate/nuo-go-certificate.crt\");\n";
		srv << "    QString keyFile(\"../ssl-certificate/nuo-go-private.key\");\n";
		srv << "    WsServer server(WsServer::ServerType::RequestAndResponse, \"WsServer_req_and_rep_console\", port, certFile, keyFile, &a);\n";
		srv << "\n";
		srv << "    QObject::connect(&server, &WsServer::incommigProtoMessage, &processor, &IncomingMessageProcessor::processIncomingMessage, Qt::DirectConnection);\n";
		srv << "\n";

		fproc_h << "    // Entity number: " << entityNum << "\n";
		fproc_h << "    Bytech::Hulk::" << item->name << " _v" << item->name << "proto;\n";
		fproc_h << "    // Don't generate db objects until db data is clear\n";
		fproc_h << "    " << item->name << " _v" << item->name << "db;\n";

		fproc_b << "        case " << entityNum << ":\n";
		fproc_b << "            _v"
				<< item->name
				<< "proto.ParseFromArray (message, message.size());\n";
		fproc_b << "            std::cerr << \"" << item->name << " received.\" << std::endl;\n";

		for (int i = 0; fitb != fite; ++fitb, ++i) {
			sys::conf::block_t * field = fitb->second;

			fproc_b << "            // type '" << field->values["type"] << "'\n";
			fproc_b << "            std::cerr << \""
					<< field->name
					<< ": \" << "
					<< "_v" << item->name << "proto." << sys::lower(field->name)
					<< "() << std::endl;\n";
		}
		fproc_b << "            break;\n";

		fproc_h << "};\n";
		fproc_h << "\n";
		fproc_h << "#endif // INCOMINGMESSAGEPROCESSOR_H\n";

		fproc_b << "        case 9999:\n";
		fproc_b << "            ::exit (0);\n";
		fproc_b << "            break;\n";
		fproc_b << "        default:\n";
		fproc_b << "            qDebug() << \"Entidad equivocada.\";\n";
		fproc_b << "            ::exit (0);\n";
		fproc_b << "            break;\n";

		fproc_b << "        }\n";
		fproc_b << "\n";
		fproc_b << "    qDebug() << \"Response sent:\" << message.toHex();\n";
		fproc_b << "    socket->sendBinaryMessage(message);\n";
		fproc_b << "}\n";

		srv << "    return a.exec();\n";
		srv << "}\n";

	}

	//
	// Create protocol buffer messages
	//
	void generator::create_proto (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		conf::block_t * lib = _conf.find(gene.c_str());

		// Protocol buffer header
		_po =  "// -------------------------------------------------------------------------\n";
		_po += "// Do NOT modify this file, it's being generated so it WILL BE OVERWRITED!!\n";
		_po += "// You have been warned.\n";
		_po += "// -------------------------------------------------------------------------\n";
		_po += "\n";
		_po += "syntax = \"proto2\";\n";
		_po += "package Bytech.Hulk;\n";
		_po += "\n";
		_po += "import \"base.proto\";\n";
		_po += "import \"Hulk.enums.proto\";\n";
		_po += "\n";

		_po += "message HulkFilter {\n";
		_po += "    //\n";
		_po += "    // Ranges delimited with semicolon (20140101;20150101)\n";
		_po += "    // Bool: 0 false, 1 true\n";
		_po += "    // Dates: YYYYMMDDHHMMSS\n";
		_po += "    // Thousand separator: . (dot)\n";
		_po += "    //\n";
		_po += "    required string Field = 1;\n";
		_po += "    required string ValueFrom = 2;\n";
		_po += "    required string ValueTo = 3;\n";
		_po += "}\n";
		_po += "\n";
		_po += "message BaseHeader {\n";
		_po += "    required Bytech.Base.MessageHeader Header = 1;\n";
		_po += "    optional string SessionId = 2;\n";
		_po += "}\n";
		_po += "\n";
		_po += "message MessageExchange {\n";
		_po += "    required BaseHeader Header = 1;\n";
		_po += "}\n";
		_po += "\n";
		_po += "message HulkResult {\n";
		_po += "    required Bytech.Hulk.BaseHeader Header = 1;\n";
		_po += "    required Bytech.Base.Result Result = 2;\n";
		_po += "}\n";
		_po += "\n";
		_po += "message HulkGetAll {\n";
		_po += "    required Bytech.Hulk.BaseHeader Header = 1;\n";
		_po += "    required Bytech.Hulk.Enums.HulkEntityType QueryEntity = 2;\n";
		_po += "    repeated HulkFilter Filters = 3;\n";
		_po += "}\n";
		_po += "\n";
		_po += "message HulkGetById {\n";
		_po += "    required Bytech.Hulk.BaseHeader Header = 1;\n";
		_po += "    required Bytech.Hulk.Enums.HulkEntityType QueryEntity = 2;\n";
		_po += "    repeated int32 Id = 3;\n";
		_po += "}\n";
		_po += "\n";

		_po += "message HulkDelete {\n";
		_po += "    required Bytech.Hulk.BaseHeader Header = 1;\n";
		_po += "    required int32 Id = 2;\n";
		_po += "}\n\n";

		conf::block_t::mapblock_t::iterator itb = lib->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = lib->subelements.end();

		for (; itb != ite; ++itb) {
			logp (sys::e_debug, itb->second->name);
			_ch = "";
			_cb = "";
			create_proto_message (itb->second);
		}

		// Protocol buffer
		std::string proto_fname (_path + "/" + name + ".proto");
		std::ofstream proto;

		proto.open (proto_fname);
		if (! proto.is_open()) {
			logp (sys::e_crit, "NO PROTO FILE!!!");
			throw file ("Can't create proto file", __FILE__, __LINE__);
		}

		_pd += "}\n\n";

		proto << _po << "\n"; // Don't add delete fields << _pd;
		// Protocol buffer end
	}

	void generator::create_proto_message (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();

		std::string name (item->name);

		_po += "message ";
		_po += name;
		_po += " {\n";

		create_proto_fields (item->subelements["fields"]);

		_po += "}\n\n";
	}

	void generator::create_proto_fields (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		int i = 1;

		conf::block_t::mapblock_t::iterator itb = item->subelements.begin();
		conf::block_t::mapblock_t::iterator ite = item->subelements.end();

		logp (sys::e_debug, "\t" << item->name);

		_po += "\t// Cabecera de todo mensaje\n";
		_po += "\trequired Bytech.Hulk.BaseHeader Header = 1;\n";

		for (; itb != ite; ++itb) {
			if (! itb->second) {
				logp (sys::e_debug, "\t\tNo field!");
				break;
			}
			create_proto_field (itb->second, ++i);
		}

	}

	void generator::create_proto_field (sys::conf::block_t * item, int num)
	{
		flog ();

		std::string type = item->values["type"];
		std::string primary = item->values["primary"];
		std::string not_null = item->values["not_null"];
		std::string def = item->values["default"];
		std::string size = item->values["size"];
		
		logp (sys::e_debug, "\t\t\tField: "
			  << item->name << ", "
			  << type << ", "
			  << primary << ", "
			  << not_null << ", "
			  << def << ", "
			  << size << ".");

		_po += "\t";

		if (not_null == "true") {
			_po += "required ";
		} else {
			_po += "optional ";
		}

		if (type == "integer")
			_po += "int32";
		else if (type == "nvarchar")
			_po += "string";
		else if (type == "bit")
			_po += "int32";
		else if (type == "datetime")
			_po += "int32";
		else if (type == "smallint")
			_po += "int32";
		else if (type == "blob")
			_po += "bytes";

		_po += " ";
		_po += item->name;
		_po += " = ";
		_po += sys::itoa(num);
		_po += ";\n";

		// Optional fields for Delete message
		_pd += "\toptional " + item->name + " = " + sys::itoa(num) + ";\n";

	}

	void generator::create_proto_enums (sys::conf::block_t * item)
	{
		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);

		sys::conf::block_t * lib = _conf.find(gene.c_str());

		// Protocol buffer header
		_po =  "// -------------------------------------------------------------------------\n";
		_po += "// Do NOT modify this file, it's being generated so it WILL BE OVERWRITED!!\n";
		_po += "// You have been warned.\n";
		_po += "// -------------------------------------------------------------------------\n";
		_po += "\n";
		_po += "syntax = \"proto2\";\n";
		_po += "package Bytech.Hulk.Enums;\n";
		_po += "\n";
		_po += "\n";

		_po += "enum HulkActions {\n";
		_po += "    GetAll = 1;\n";
		_po += "    GetById = 2;\n";
		_po += "    Insert = 3;\n";
		_po += "    Update = 4;\n";
		_po += "    Delete = 5;\n";
		_po += "}\n";
		_po += "\n";

		sys::conf::block_t::mapblock_t::iterator itb = lib->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator ite = lib->subelements.end();

		_po += "enum HulkEntityType {\n";
		_po += "    HulkGetAll = 1;\n";
		_po += "    HulkGetById = 2;\n";
		for (int i = 3; itb != ite; ++itb, ++i) {
			_po += "    " + itb->second->name + " = " + sys::itoa(i) + ";\n";
		}

		// Protocol buffer
		std::string proto_fname (_path + "/Hulk.enums.proto");
		std::ofstream proto;

		proto.open (proto_fname);
		if (! proto.is_open()) {
			logp (sys::e_crit, "NO PROTO FILE!!!");
			throw file ("Can't create proto file", __FILE__, __LINE__);
		}

		proto << _po;
		proto << "}\n\n";
	}

	void generator::create_server (sys::conf::block_t * item)
	{
		using namespace sys;
		flog ();
		logp (sys::e_debug, "In file: '" << item->name << "'.");
		logp (sys::e_debug, "In file: '" << item->values["name"] << "'.");
		bool server = item->values["gen_server"] == "true";

		if (! server) {
			logp (sys::e_debug, "NO SERVER!!!");
			logp (sys::e_debug, "'" << item->values["gen_servers"] << "'");
			return;
		}

		std::string name = item->values["name"];
		std::string gene = ("sys/" + item->values["gen"]);
		std::string test_path = item->values["tests_dir"];

		conf::block_t * lib = _conf.find(gene.c_str());
		auto itb = lib->subelements.begin();
		auto ite = lib->subelements.end();

		std::string tmp_path (_path + "/");

		std::string qtpro_name (tmp_path + "server.pro");
		std::string fname (tmp_path + "Server.cpp");
		std::string proc_header (tmp_path + "incomingmessageprocessor.h");
		std::string proc_body (tmp_path + "incomingmessageprocessor.cpp");

		std::ofstream pro;
		logp (sys::e_debug, ".pro file: " << fname);

		sys::stat_t stat_qtpro (qtpro_name);

		if (stat_qtpro) {
			sys::file_system::safe_mv (qtpro_name, qtpro_name + ".old");
		}

		pro.open (qtpro_name);
		if (! pro.is_open()) {
			logp (sys::e_crit, "NO .pro FILE!!!");
			throw file ("Can't create .pro file", __FILE__, __LINE__);
		}

		std::ofstream srv;
		sys::stat_t stat_srv (fname);

		if (stat_srv) {
			sys::file_system::safe_mv (fname, fname + ".old");
		}

		srv.open (fname);
		if (! srv.is_open()) {
			logp (sys::e_crit, "NO Server.cpp FILE!!!");
			throw file ("Can't create Server.cpp file", __FILE__, __LINE__);
		}

		std::ofstream fproc_h;
		sys::stat_t stat_header (proc_header);

		if (stat_header) {
			sys::file_system::safe_mv (proc_header, proc_header + ".old");
		}

		fproc_h.open (proc_header);
		if (! fproc_h.is_open()) {
			logp (sys::e_crit, "NO incomingmessageprocessor.srv.cpp FILE!!!");
			throw file ("Can't create incomingmessageprocessor.cpp file", __FILE__, __LINE__);
		}

		std::ofstream fproc_b;
		sys::stat_t stat_body (proc_body);

		if (stat_body) {
			sys::file_system::safe_mv (proc_body, proc_body + ".old");
		}

		fproc_b.open (proc_body);
		if (! fproc_b.is_open()) {
			logp (sys::e_crit, "NO incomingmessageprocessor.srv.cpp FILE!!!");
			throw file ("Can't create incomingmessageprocessor.cpp file", __FILE__, __LINE__);
		}

		/* Qt .pro file */

		pro << "#-------------------------------------------------\n";
		pro << "#\n";
		pro << "# Project created by ByTech \n";
		pro << "#\n";
		pro << "#-------------------------------------------------\n";
		pro << "\n";
		pro << "#include($$PWD/../Hulk.pri)\n";
		pro << "\n";
		pro << "QT -= gui\n";
		pro << "QT += websockets\n";
		pro << "TARGET = server\n";
		pro << "CONFIG += console\n";
		pro << "CONFIG -= app_bundle\n";
		pro << "TEMPLATE = app\n";
		pro << "include($$PWD/../testers-common.pri)\n";
		pro << "\n";
		pro << "CONFIG( debug, debug|release ) {\n";
		pro << "    unix:!macx: LIBS += -L$$BY_LIBOUTPUTDIR -lbytech-hulk-wscommunicationsd\n";
		pro << "} else {\n";
		pro << "    unix:!macx: LIBS += -L$$BY_LIBOUTPUTDIR -lbytech-hulk-wscommunications\n";
		pro << "}\n";
		pro << "\n";
		pro << "INCLUDEPATH += $$PWD/../../libs/boost/include\n";
		pro << "DEPENDPATH += $$PWD/../../libs/boost/include\n";
		pro << "LIBS += -L$$PWD/../../libs/boost/lib -lboost_serialization\n";
		pro << "\n";
		pro << "INCLUDEPATH += $$PWD/../../libs/qxorm/include\n";
		pro << "DEPENDPATH += $$PWD/../../libs/qxorm/include\n";
		pro << "unix:!macx: LIBS += -L$$PWD/../../libs/qxorm/lib -lQxOrmd\n";
		pro << "\n";
		pro << "LIBS += -L$$PWD/../protobuf/src -lprotobuf -lprotoc\n";
		pro << "\n";
		pro << "LIBS += -L$$PWD/../../bytech-hulk/bytech-hulk-entities/bin -lHulkd\n";
		pro << "message($$LIBS)\n";
		pro << "\n";
		pro << "\n";
		pro << "INCLUDEPATH += $$PWD/../bytech-hulk-wscommunications\n";
		pro << "DEPENDPATH += $$PWD/../output/pc/debug\n";
		pro << "DEPENDPATH += $$PWD/output/pc/debug\n";
		pro << "INCLUDEPATH += $$PWD/../include\n";
		pro << "INCLUDEPATH += $$PWD/..\n";
		pro << "DEPENDPATH += $$PWD/../bytech-hulk-wscommunications\n";
		pro << "\n";

		/* incoming message processor header */

		fproc_h << "//\n";
		fproc_h << "//\n";
		fproc_h << "//\n";
		fproc_h << "#ifndef INCOMINGMESSAGEPROCESSOR_H\n";
		fproc_h << "#define INCOMINGMESSAGEPROCESSOR_H\n";
		fproc_h << "\n";
		fproc_h << "#include <stdlib.h>\n";
		fproc_h << "#include <iostream>\n";
		fproc_h << "#include <QObject>\n";
		fproc_h << "#include <QWebSocket>\n";
		fproc_h << "#include <QDebug>\n";
		fproc_h << "#include <QByteArray>\n";
		fproc_h << "#include \"wsserver.h\"\n";
		fproc_h << "#include \"../bytech-hulk-proto/Hulk.pb.h\"\n";
		fproc_h << "#include \"../bytech-hulk-proto/base.pb.h\"\n";
		fproc_h << "#include \"../bytech-hulk-proto/base.enums.pb.h\"\n";
		//fproc_h << "#include \"../bytech-hulk-entities/include/hulk_all_include.gen.h\"\n";
		//fproc_h << "#include \"../bytech-hulk-/entityprocessbase.h\"\n";
		//fproc_h << "#include \"requestparams.h\"\n";
		//fproc_h << "#include \"bymodule.h\"\n";

 		for (int i = 3; itb != ite; ++itb, ++i) {
			sys::conf::block_t * item = itb->second;
			sys::conf::block_t * fields = item->subelements["fields"];
			sys::conf::block_t::mapblock_t::iterator fitb
				= fields->subelements.begin();
			sys::conf::block_t::mapblock_t::iterator fite
				= fields->subelements.end();
			fproc_h << "#include \"../bytech-hulk-business/" << item->name << "Business.h\"\n";
		}

		fproc_h << "\n";
		fproc_h << "class IncomingMessageProcessor : public QObject\n";
		fproc_h << "{\n";
		fproc_h << "    Q_OBJECT\n";
		fproc_h << "public:\n";
		fproc_h << "    typedef QByteArray(IncomingMessageProcessor::*Process)(const QByteArray &params);\n";
		fproc_h << "    explicit IncomingMessageProcessor(QObject *parent = 0);\n";
		fproc_h << "    virtual ~IncomingMessageProcessor();\n";
		fproc_h << "\n";
		fproc_h << "public slots:\n";
		fproc_h << "    void processIncomingMessage(QWebSocket *socket, const QByteArray &message);\n";
		fproc_h << "signals:\n";
		fproc_h << "\n";
		fproc_h << "public slots:\n";
		fproc_h << "private:\n";
		fproc_h << "    QHash<int, Process> _processorBaseMap;\n";

		fproc_h << "    Bytech::Hulk::BaseHeader _header;\n";
		fproc_h << "    Bytech::Hulk::HulkGetAll _hulkGetAll;\n";
		fproc_h << "    Bytech::Hulk::HulkGetById _hulkGetById;\n";
		fproc_h << "\n";

		/* incoming message processor body */

		fproc_b << "//\n";
		fproc_b << "//\n";
		fproc_b << "//\n";
		fproc_b << "#include \"incomingmessageprocessor.h\"\n";
		fproc_b << "\n";
		fproc_b << "void IncomingMessageProcessor::processIncomingMessage(QWebSocket* socket, const QByteArray &message)\n";
		fproc_b << "{\n";
		fproc_b << "    static int i = 0;\n";
		fproc_b << "    if (!socket)\n";
		fproc_b << "    {\n";
		fproc_b << "        qDebug() << \"Socket sender null pointer:\";\n";
		fproc_b << "        return;\n";
		fproc_b << "    }\n";
		fproc_b << "\n";
		fproc_b << "    qDebug () << \"Parsing message.\";\n";
		fproc_b << "    _hulkGetAll.ParseFromArray (message, message.size());\n";
		fproc_b << "    qDebug () << \"Calling processor.\";\n";
		fproc_b << "    qDebug () << \"Value to call: \" << _hulkGetAll.queryentity();\n";
		fproc_b << "    Process fn = _processorBaseMap[_hulkGetAll.queryentity()];\n";

		itb = lib->subelements.begin();
 		for (int i = 3; itb != ite; ++itb, ++i) {
			sys::conf::block_t * item = itb->second;
			sys::conf::block_t * fields = item->subelements["fields"];
			sys::conf::block_t::mapblock_t::iterator fitb
				= fields->subelements.begin();
			sys::conf::block_t::mapblock_t::iterator fite
				= fields->subelements.end();
			fproc_b << "    qDebug() << \"Bytech::Hulk::Enums::HulkEntityType::" + item->name + ": \" ";
			fproc_b << "             << Bytech::Hulk::Enums::HulkEntityType::" + item->name + ";\n";
		}

		fproc_b << "\n";
		fproc_b << "    if (fn) {\n";
		fproc_b << "        (this->*fn)(message);\n";
		fproc_b << "    }\n";
		fproc_b << "    //(this->*_processorBaseMap[_hulkGetAll.queryentity()])(message);\n";
		fproc_b << "\n";
		//fproc_b << "    switch (_header.header().entity()) {\n";


		/* standalone server */

		srv << "// -------------------------------------------------\n";
		srv << "// -------------------------------------------------\n";
		srv << "// -------------------------------------------------\n";
		srv << "#include <QObject>\n";
		srv << "#include <QWebSocket>\n";
		srv << "#include <QDebug>\n";
		srv << "#include <QByteArray>\n";
		srv << "#include <QxOrm.h>\n";
		srv << "#include <QCoreApplication>\n";
		srv << "#include <QString>\n";
		//srv << "#include \"../bytech-hulk-entities/include/hulk_all_include.gen.h\"\n";
		//srv << "#include \"base.pb.h\"\n";
		//srv << "#include \"base.enums.pb.h\"\n";
		//srv << "#include \"Hulk.pb.h\"\n";
		//srv << "#include \"entityprocessbase.h\"\n";
		//srv << "#include \"requestparams.h\"\n";
		//srv << "#include \"bymodule.h\"\n";
		srv << "#include \"wsserver.h\"\n";
		srv << "#include \"incomingmessageprocessor.h\"\n";
		srv << "\n";
		srv << "\n";
		srv << "int main (int argc, char ** argv)\n";
		srv << "{\n";
		srv << "\n";
		srv << "    int port = 9090;\n";
		srv << "    if (argc > 1)\n";
		srv << "        port = atoi(argv[1]);\n";
		srv << "\n";
		srv << "    QCoreApplication a(argc, argv);\n";
		srv << "\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setDriverName(\"QSQLITE\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setDatabaseName(\"./hulk.db\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setHostName(\"localhost\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setUserName(\"root\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setPassword(\"\");\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(true);\n";
		srv << "    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(true);\n";
		srv << "\n";
		srv << "    IncomingMessageProcessor processor(&a);\n";
		srv << "    QString certFile(\"../ssl-certificate/nuo-go-certificate.crt\");\n";
		srv << "    QString keyFile(\"../ssl-certificate/nuo-go-private.key\");\n";
		srv << "    WsServer server(WsServer::ServerType::RequestAndResponse, \"WsServer_req_and_rep_console\", port, certFile, keyFile, &a);\n";
		srv << "\n";
		srv << "    QObject::connect(&server, &WsServer::incommigProtoMessage, &processor, &IncomingMessageProcessor::processIncomingMessage, Qt::DirectConnection);\n";
		srv << "\n";

		std::string db_headers;
		std::string db_sources;

		std::string contracts_h; // headers
		std::string contracts_b; // bodies

		std::string constructor;
		std::string funcs;

		itb = lib->subelements.begin();
 		for (int i = 3; itb != ite; ++itb, ++i) {
			sys::conf::block_t * item = itb->second;
			sys::conf::block_t * fields = item->subelements["fields"];
			sys::conf::block_t::mapblock_t::iterator fitb
				= fields->subelements.begin();
			sys::conf::block_t::mapblock_t::iterator fite
				= fields->subelements.end();

			//std::string proc_body (tmp_path + "/" + ".test.html");

			db_headers += "    ../bytech-hulk-entities/include/" + itb->second->name + ".gen.h \\\n";
			db_sources += "    ../bytech-hulk-entities/src/" + itb->second->name + ".gen.cpp \\\n";

			contracts_h += "    " + item->name + "/" + item->name + "Business.h \\\n";
			contracts_b += "    " + item->name + "/" + item->name + "Business.cpp \\\n";

			constructor += "    _processorBaseMap.insert(Bytech::Hulk::Enums::HulkEntityType::" + item->name + ", \n";
			constructor += "        &IncomingMessageProcessor::" + item->name + "BaseProcess);\n";
			
			fproc_h << "    QByteArray " + item->name + "BaseProcess (const QByteArray & params);\n";

			funcs += "QByteArray IncomingMessageProcessor::" + item->name + "BaseProcess (const QByteArray & params)\n";
			funcs += "{\n";
			funcs += "    " + item->name + "Business processor;\n";
			funcs += "    return processor.Process(params);\n";
			funcs += "}\n";
			funcs += "\n";
#if 0
			fproc_h << "    // Entity number: " << i << "\n";
			fproc_h << "    Bytech::Hulk::" << item->name << " _v" << item->name << "proto;\n";
			fproc_h << "    // Don't generate db objects until db data is clear\n";
			fproc_h << "    //" << item->name << " _v" << item->name << "db;\n";
#endif
#if 0
			fproc_b << "        case " << i << ": {\n";
			fproc_b << "            std::cerr << \"" << item->name << " received.\" << std::endl;\n";
			fproc_b << "            switch (_header.header().action()) {\n";
			fproc_b << "                case Bytech::Hulk::Enums::HulkActions::GetAll: {\n";
			fproc_b << "                    std::string msg;\n";
			fproc_b << "                    QByteArray arr;\n";
			fproc_b << "                    _v" << item->name << "proto.set_id(++i);\n";
			fproc_b << "                    arr.resize (_v" << item->name << "proto.ByteSize());\n";
			fproc_b << "                    _v" << item->name << "proto.SerializeToArray (arr.data(), arr.size());\n";
			fproc_b << "                    QByteArray byteArray (msg.c_str());\n";
			fproc_b << "                    socket->sendBinaryMessage(byteArray);\n";
			fproc_b << "                }\n";
			fproc_b << "                break;\n";
			fproc_b << "                case Bytech::Hulk::Enums::HulkActions::GetById: {\n";
			fproc_b << "                    //_v" << item->name << "proto.set_id(++i);\n";
			fproc_b << "                    qDebug() << \"Response sent:\" << message.toHex();\n";
			fproc_b << "                    socket->sendBinaryMessage(message);\n";
			fproc_b << "                }\n";
			fproc_b << "                break;\n";
			fproc_b << "                case Bytech::Hulk::Enums::HulkActions::Insert: {\n";
			fproc_b << "                    //_v" << item->name << "proto.set_id(++i);\n";
			fproc_b << "                    qDebug() << \"Response sent:\" << message.toHex();\n";
			fproc_b << "                    socket->sendBinaryMessage(message);\n";
			fproc_b << "                }\n";
			fproc_b << "                break;\n";
			fproc_b << "                case Bytech::Hulk::Enums::HulkActions::Update: {\n";
			fproc_b << "                    //_v" << item->name << "proto.set_id(++i);\n";
			fproc_b << "                    qDebug() << \"Response sent:\" << message.toHex();\n";
			fproc_b << "                    socket->sendBinaryMessage(message);\n";
			fproc_b << "                }\n";
			fproc_b << "                break;\n";
			fproc_b << "                case Bytech::Hulk::Enums::HulkActions::Delete: {\n";
			fproc_b << "                    //_v" << item->name << "proto.set_id(++i);\n";
			fproc_b << "                    qDebug() << \"Response sent:\" << message.toHex();\n";
			fproc_b << "                    socket->sendBinaryMessage(message);\n";
			fproc_b << "                }\n";
			fproc_b << "                break;\n";
			fproc_b << "            }\n";
#endif
#if 0
			for (int i = 0; fitb != fite; ++fitb, ++i) {
				sys::conf::block_t * field = fitb->second;

				fproc_b << "            // type '" << field->values["type"] << "'\n";
				fproc_b << "            std::cerr << \""
						<< field->name
						<< ": \" << "
						<< "_v" << item->name << "proto." << sys::lower(field->name)
						<< "() << std::endl;\n";
				fproc_b << "            }\n";

				if (field->values["type"] == "nvarchar") {
				} else if (field->values["type"] == "integer") {
					srv << "    // type '" << field->values["type"] << "'\n";
					srv << "    long integer" << sys::itoa(++i) << " = 10;\n";
					srv << "    elem.set" << field->name << "("
						<< "integer" << sys::itoa(i) << ");\n";
				} else if (field->values["type"] == "bit") {
					srv << "    // type '" << field->values["type"] << "'\n";
					srv << "    int integer" << sys::itoa(++i) << " = 10;\n";
					srv << "    elem.set" << field->name << "("
						<< "integer" << sys::itoa(i) << ");\n";
				} else if (field->values["type"] == "datetime") {
					srv << "    // type '" << field->values["type"] << "'\n";
					srv << "    QDateTime dt" << sys::itoa(++i) << ";\n";
					srv << "    elem.set" << field->name << "("
						<< "dt" << sys::itoa(i) << ");\n";
				} else if (field->values["type"] == "blob") {
					srv << "    // type '" << field->values["type"] << "'\n";
					srv << "    char * ch" << sys::itoa(++i) << " = \"abc\";\n";
					srv << "    elem.set" << field->name << "("
						<< "ch" << sys::itoa(i) << ");\n";
				} else {
					srv << "    // type '" << field->values["type"] << "'\n";
					srv << "    long other" << sys::itoa(++i) << " = 10;\n";
					srv << "    elem.set" << field->name << "("
						<< "other" << sys::itoa(i) << ");\n";
				}
			}
			fproc_b << "            }\n";
			fproc_b << "            break;\n";
#endif
		}

		fproc_b << "}\n\n";

		// remove trailing backslash, newline and space
		db_headers = db_headers.substr(0, db_headers.size() - 3);
		db_sources = db_sources.substr(0, db_sources.size() - 3);
		contracts_h = contracts_h.substr(0, contracts_h.size() - 3);
		contracts_b = contracts_b.substr(0, contracts_b.size() - 3);

		pro << "HEADERS += incomingmessageprocessor.h ";
		pro << "base.pb.h ";
		pro << "base.enums.pb.h ";
		pro << "Hulk.pb.h ";
		pro << "Hulk.enums.pb.h ";
		pro << "entityprocessbase.h ";
		pro << "protohelper.h ";
		pro << "protobuf.h ";
		pro << "requestparams.h ";
		pro << "bymodule.h ";
		pro << "protobase \\\n";
		// don't include database objects until needed
		//pro << db_headers;
		pro << contracts_h;
		pro << "\n";
		pro << "\n";

		pro << "SOURCES += Server.cpp ";
		pro << "incomingmessageprocessor.cpp ";
		pro << "base.pb.cc base.enums.pb.cc protohelper.cpp ";
		pro << "Hulk.enums.pb.cc Hulk.pb.cc ";
		pro << "protobuf.cpp bymodule.cpp \\\n";
		// don't include database objects until needed
		//pro << db_sources;
		pro << contracts_b;
		pro << "\n";
		pro << "\n";
		pro << "\n";


		fproc_h << "};\n";
		fproc_h << "\n";
		fproc_h << "#endif // INCOMINGMESSAGEPROCESSOR_H\n";
#if 0
		fproc_b << "        case 9999:\n";
		fproc_b << "            ::exit (0);\n";
		fproc_b << "            break;\n";

		fproc_b << "        }\n";
#endif

		fproc_b << "IncomingMessageProcessor::IncomingMessageProcessor(QObject *parent) : QObject(parent)\n";
		fproc_b << "{\n";
		fproc_b << constructor;
		fproc_b << "\n";
		fproc_b << "}\n";
		fproc_b << "\n";

		fproc_b << "IncomingMessageProcessor::~IncomingMessageProcessor()\n";
		fproc_b << "{\n";
		fproc_b << "\n";
		fproc_b << "}\n";
		fproc_b << "\n";

		fproc_b << funcs;
		fproc_b << "\n";

		srv << "    return a.exec();\n";
		srv << "}\n";

	}

} // end namespace app
