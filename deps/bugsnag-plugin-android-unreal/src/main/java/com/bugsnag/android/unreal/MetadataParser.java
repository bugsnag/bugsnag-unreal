package com.bugsnag.android.unreal;

import java.io.ByteArrayInputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.util.JsonReader;
import android.util.JsonToken;

class MetadataParser {

  private static String ERROR_MESSAGE = "Input must be a JSON object";

  static Map<String, Object> parse(byte[] jsonData) throws IOException {
    if (jsonData == null) {
      throw new IOException(ERROR_MESSAGE);
    }
    ByteArrayInputStream stream = new ByteArrayInputStream(jsonData);
    JsonReader reader = new JsonReader(new InputStreamReader(stream));
    if (reader.peek() != JsonToken.BEGIN_OBJECT) {
      // Early exit if the top level element isn't an object
      reader.close();
      throw new IOException(ERROR_MESSAGE);
    }

    Map<String, Object> metadata = parseJsonObject(reader);
    reader.close();
    if (metadata == null || !(metadata instanceof Map)) {
      throw new IOException(ERROR_MESSAGE);
    }
    return metadata;
  }

  private static Map<String, Object> parseJsonObject(JsonReader reader) throws IOException {
    HashMap<String, Object> items = new HashMap<>();
    reader.beginObject();
    while (reader.hasNext()) {
      String key = reader.nextName();
      items.put(key, parseJsonValue(reader));
    }
    reader.endObject();
    return items;
  }

  private static List parseJsonArray(JsonReader reader) throws IOException {
    ArrayList<Object> items = new ArrayList<>();
    reader.beginArray();
    while (reader.hasNext()) {
      items.add(parseJsonValue(reader));
    }
    reader.endArray();
    return items;
  }

  private static Object parseJsonValue(JsonReader reader) throws IOException {
    switch (reader.peek()) {
      case BOOLEAN:
        return reader.nextBoolean();
      case BEGIN_OBJECT:
        return parseJsonObject(reader);
      case BEGIN_ARRAY:
        return parseJsonArray(reader);
      case NUMBER:
        return reader.nextDouble();
      case STRING:
        return reader.nextString();
      default:
        // NULL, invalid tokens
        reader.skipValue();
        return null;
    }
  }
}
