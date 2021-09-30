package com.bugsnag.android.unreal;

import java.io.ByteArrayOutputStream;
import java.io.OutputStreamWriter;
import java.io.IOException;
import java.util.List;
import java.util.Map;

import android.util.JsonToken;
import android.util.JsonWriter;

class MetadataSerializer {

  static byte[] serialize(Map<String, Object> input) throws IOException {
    if (input == null || !(input instanceof Map)) {
      throw new IOException("Input must be a Map<String, Object>");
    }
    ByteArrayOutputStream stream = new ByteArrayOutputStream();
    JsonWriter writer = new JsonWriter(new OutputStreamWriter(stream));
    writeObject(writer, input);
    writer.close();

    return stream.toByteArray();
  }

  private static void writeObject(JsonWriter writer, Map<String, Object> object) throws IOException {
    writer.beginObject();
    for (Map.Entry<String, Object> entry : object.entrySet()) {
      if (entry.getKey() instanceof String) {
        writer.name(entry.getKey());
        writeValue(writer, entry.getValue());
      }
    }
    writer.endObject();
  }

  private static void writeList(JsonWriter writer, List<Object> list) throws IOException {
    writer.beginArray();
    for (Object value : list) {
      writeValue(writer, value);
    }
    writer.endArray();
  }

  private static void writeValue(JsonWriter writer, Object value) throws IOException {
    if (value instanceof Map) {
      writeObject(writer, (Map<String, Object>)value);
    } else if (value instanceof Boolean) {
      writer.value((boolean)value);
    } else if (value instanceof Number) {
      writer.value((Number)value);
    } else if (value instanceof String) {
      writer.value((String)value);
    } else if (value instanceof List) {
      writeList(writer, (List<Object>)value);
    } else if (value == null) {
      writer.nullValue();
    } else {
      throw new IOException("unable to parse value of type " + value.getClass().getName());
    }
  }
}
