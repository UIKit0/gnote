

#include <map>

#include "sharp/string.hpp"
#include "note.hpp"
#include "tag.hpp"
#include "sharp/foreach.hpp"

namespace gnote {

	const char * Tag::SYSTEM_TAG_PREFIX = "system:";

	class Tag::NoteMap
		: public std::map<std::string, Note::Ptr>
	{
	};

	Tag::Tag(const std::string & _name)
		: m_issystem(false)
		, m_isproperty(false)
		, m_notes(new NoteMap)
	{
		set_name(_name);
	}

	Tag::~Tag()
	{
		delete m_notes;
	}

	void Tag::add_note(Note & note)
	{
		if(m_notes->find(note.uri()) == m_notes->end()) {
			(*m_notes)[note.uri()] = note.shared_from_this();
		}
	}


	void Tag::remove_note(const Note & note)
	{
		NoteMap::iterator iter = m_notes->find(note.uri());
		if(iter != m_notes->end()) {
			m_notes->erase(iter);
		}
	}


	void Tag::set_name(const std::string & value)
	{
		if (!value.empty()) {
			std::string trimmed_name = sharp::string_trim(value);
			if (!trimmed_name.empty()) {
				m_name = trimmed_name;
				m_normalized_name = sharp::string_to_lower(trimmed_name);
				if(sharp::string_starts_with(m_normalized_name, SYSTEM_TAG_PREFIX)) {
					m_issystem = true;
				}
				std::vector<std::string> splits;
				sharp::string_split(splits, value, ":");
				m_isproperty  = (splits.size() >= 3);
			}
		}
	}


	int Tag::popularity() const
	{
		return m_notes->size();
	}


	void Tag::remove_all_notes()
	{
		foreach (const NoteMap::value_type & value, *m_notes) {
			value.second->remove_tag(*this);
		}
	}
}

