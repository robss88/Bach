


struct VoicingOperation
{
    String type;
    Array<int> drop;
    int index;
    Array<int> octaves;

    static VoicingOperation fromJSON(const var& json)
    {
        VoicingOperation op;
        op.type = json["type"].toString();
        
        if (op.type == "drop")
        {
            for (auto& item : *json["drop"].getArray())
            {
                op.drop.add(item);
            }
        }
        else if (op.type == "duplicate")
        {
            op.index = json["index"];
            for (auto& octave : *json["octaves"].getArray())
            {
                op.octaves.add(octave);
            }
        }
        
        return op;
    }
};

struct Voicing
{
    String name;
    Array<String> tags;
    Array<VoicingOperation> operations;

    static Voicing fromVar(const var& json)
    {
        Voicing cv;
        cv.name = json["name"].toString();
        for (auto& tag : *json["tags"].getArray())
        {
            cv.tags.add(tag.toString());
        }
        for (auto& op : *json["operations"].getArray())
        {
            cv.operations.add(VoicingOperation::fromJSON(op));
        }
        return cv;
    }
    static Voicing fromJSON(const String& jsonString)
    {
        var json = JSON::parse(jsonString);
        return fromVar(json);
    }
    
    static Array<Voicing> fromJSONArray(const String& jsonString)
    {
        Array<Voicing> voicings;
        juce::var parsedJson = juce::JSON::parse(jsonString);
        juce::Array<juce::var>* jsonArray = parsedJson.getArray();
        
        for (auto json : *jsonArray)
        {
            voicings.add(Voicing::fromVar(json));
        }
        return voicings;
    }
};
