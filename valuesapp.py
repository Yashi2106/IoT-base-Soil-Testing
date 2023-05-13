#!/usr/bin/env python
# coding: utf-8

# In[1]:



import requests
import sys
import numpy as np
import joblib
import streamlit as st
import ssl
ssl._create_default_https_context = ssl._create_unverified_context()
import warnings
warnings.filterwarnings('ignore', message="X does not have valid feature names, but DecisionTreeClassifier was fitted with feature names")

CHANNEL_ID = '2102197'
READ_API_KEY = 'XPNL5CQ9NK9VSN9R'

model = joblib.load('trained_model.joblib')

def get_data():
    response = requests.get(f'https://api.thingspeak.com/channels/2102197/feeds.json?api_key=XPNL5CQ9NK9VSN9R&results=1')

    if response.status_code == 200:
        data = response.json()['feeds'][0]
    else:
        data = None

    return data

def preprocess_data(data):
    N_SOIL = float(data['field1'])
    P_SOIL = float(data['field2'])
    K_SOIL = float(data['field3'])
    TEMPERATURE = float(data['field4'])
    HUMIDITY = float(data['field5'])
    ph = float(data['field6']) # fixed variable name to uppercase
    RAINFALL = float(data['field7'])
    MOISTURE = float(data['field8'])

    features = np.array([N_SOIL, P_SOIL, K_SOIL, ph, TEMPERATURE, HUMIDITY, RAINFALL, MOISTURE]) # fixed variable names

    return features

def predict_crop(features):
    prediction = model.predict_proba(features.reshape(1, -1)).argmax(axis=1)[0]
    return prediction

def main():
    st.title('Crop Recommendation System')
    st.subheader('Real-time Data from ThingSpeak')

    data = get_data()

    if data is not None:
        st.write('Last updated at', data['created_at'])
        st.write('Value obtained from ThingSpeak:')
        st.write('- N_SOIL:', data['field1'])
        st.write('- P_SOIL:', data['field2'])
        st.write('- K_SOIL:', data['field3'])
        st.write('- TEMPERATURE:', data['field4'])
        st.write('- HUMIDITY:', data['field5'])
        st.write('- pH:', data['field6'])
        st.write('- RAINFALL:', data['field7'])
        st.write('- MOISTURE:', data['field8'])
        features = preprocess_data(data)
        predicted_crop_index = predict_crop(features)
        predicted_crop = model.classes_[int(predicted_crop_index)]
        st.write('Based on the data collected, the recommended crop is:', predicted_crop)
    else:
        st.write('Error retrieving data from ThingSpeak.')

if __name__ == '__main__':
    main()


# In[2]:


import joblib
print(joblib.__version__)


# In[3]:


import streamlit
print(streamlit.__version__)


# In[4]:


import numpy
print(numpy.__version__)


# In[5]:


import requests
print(requests.__version__)


# In[6]:



import scipy

print(scipy.__version__)


# In[7]:


import sklearn
print(sklearn.__version__)


# In[ ]:




