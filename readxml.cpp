char * FileToChar(char * file_name)
{
	ifstream file(file_name);
	if (file.bad())
	{
		cout << "The file doesn't exist!" << endl;
		exit(-1);
	}

	filebuf * pbuf = file.rdbuf();
	long size_of_file = pbuf->pubseekoff(0, ios::end, ios::in);
	file.seekg(0);

	char * output = new char[size_of_file + 1];

	file.read(output, size_of_file);

	return output;
}

void SaveCharactersData(int slot_number, char * file_content)
{
	char *slot;
	string temp;
	if (slot_number == 1)
		slot = "Slot1";
	else if (slot_number == 2)
		slot = "Slot2";
	else
		slot = "Slot3";

	auto_ptr < char > zawartosc_pliku(file_content);

	xml_document <> doc;
	try {
		doc.parse < 0 >(zawartosc_pliku.get());
	}
	catch (parse_error p) {
		p.what();
	}

	xml_node <> *player_node = doc.first_node();
	for (xml_node <> *inner_node = player_node->first_node(); inner_node; inner_node = inner_node->next_sibling())
	{
		if (!strcmp(inner_node->name(), slot))
			for (xml_node <> *more_inner_node = inner_node->first_node(); more_inner_node; more_inner_node = more_inner_node->next_sibling())
			{
				if (!strcmp(more_inner_node->name(), "class"))
				{
					temp = to_string(class_id);
					more_inner_node->value(temp.c_str());
				}


				if (!strcmp(more_inner_node->name(), "lvl"))
					LVL = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "exp"))
					experience = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "posX"))
					m_PosX = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "posY"))
					m_PosY = atoi(more_inner_node->value());

				for (xml_node <> *much_more_inner_node = more_inner_node->first_node(); much_more_inner_node; much_more_inner_node = much_more_inner_node->next_sibling())
				{
					if (!strcmp(much_more_inner_node->name(), "chest"))
						;			//chest_armor = much_more_inner_node->value();
					if (!strcmp(much_more_inner_node->name(), "str"))
						m_strength = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "agi"))
						m_agility = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "sta"))
						m_stamina = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "int"))
						m_intelligence = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "spt"))
						m_spirit = atoi(much_more_inner_node->value());
				}
			}
	}
	ofstream file("CharactersData.xml");
	file << "<?xml version=\1.0\" encoding=\"utf-8?>" << endl;
	file << doc;
	doc.clear();
	file.close();
}