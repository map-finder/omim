package com.mapswithme.maps.editor;

import android.support.annotation.Nullable;

interface TimetableProvider
{
  @Nullable String getTimetables();
  void setTimetables(@Nullable String timetables);
}
