import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

st.set_page_config(layout='wide', page_title='Startup Analysis')
st.sidebar.title('Indian Phone Market Analysis')

# Dropdown for analysis selection
option = st.sidebar.selectbox(
    'Select one',
    ['Univariate Analysis', 'Bivariate Analysis']
)

# Load dataset
@st.cache_data
def load_data():
    df = pd.read_csv('D:\Education\Extra\Machine Learning\Project\Advanced Web Scraping - PhoneData\Phone_cleaned_data')  # replace with your dataset
    return df

df = load_data()

# Separate columns by type
categorical_cols = ['Brand_name','Has_5G','Has_NFC','Has_IR','Processor_brand','Processor_core','Ram','Rom','Has_fast_charging','Front_camera','Rear_camera','Memory_card_support','Memory_card_size','Os_name','Refresh_rate']
numerical_cols = ['Price','Rating','Screen_size','Battery_capacity','Processor_frequency']

# ------------------ UNIVARIATE ------------------
if option == 'Univariate Analysis':
    st.header("📊 Univariate Analysis")

    col_type = st.radio("Select column type", ["Categorical", "Numerical"])

    if col_type == "Categorical":
        col = st.selectbox("Select a categorical column", categorical_cols)
        fig, ax = plt.subplots()
        df[col].value_counts().plot(kind='bar', ax=ax)
        plt.xticks(rotation=90)
        st.pyplot(fig)

    elif col_type == "Numerical":
        col = st.selectbox("Select a numerical column", numerical_cols)
        fig, ax = plt.subplots(1, 2, figsize=(12, 4))

        # Histogram
        sns.histplot(df[col], kde=True, ax=ax[0])
        ax[0].set_title(f"Distribution of {col}")

        # Boxplot
        sns.boxplot(x=df[col], ax=ax[1])
        ax[1].set_title(f"Boxplot of {col}")

        st.pyplot(fig)

# ------------------ BIVARIATE ------------------
elif option == 'Bivariate Analysis':
    st.header("🔗 Bivariate Analysis")

    relation = st.radio("Select relationship type", [
        "Numerical vs Numerical",
        "Categorical vs Numerical",
        "Categorical vs Categorical"
    ])

    if relation == "Numerical vs Numerical":
        x_col = st.selectbox("Select X-axis (Numerical)", numerical_cols, index=0)
        y_col = st.selectbox("Select Y-axis (Numerical)", numerical_cols, index=1)
        fig, ax = plt.subplots()
        sns.scatterplot(x=df[x_col], y=df[y_col], ax=ax)
        ax.set_title(f"{x_col} vs {y_col}")
        st.pyplot(fig)

    elif relation == "Categorical vs Numerical":
        x_col = st.selectbox("Select Categorical Column", categorical_cols)
        y_col = st.selectbox("Select Numerical Column", numerical_cols)
        fig, ax = plt.subplots()
        sns.stripplot(x=df[x_col], y=df[y_col], ax=ax)
        plt.xticks(rotation=90)
        ax.set_title(f"{x_col} vs {y_col}")
        st.pyplot(fig)

    elif relation == "Categorical vs Categorical":
        x_col = st.selectbox("Select First Categorical Column", categorical_cols)
        y_col = st.selectbox("Select Second Categorical Column", categorical_cols)
        crosstab = pd.crosstab(df[x_col], df[y_col])

        fig, ax = plt.subplots(figsize=(8, 6))
        sns.heatmap(crosstab, ax=ax)
        ax.set_title(f"Heatmap of {x_col} vs {y_col}")
        st.pyplot(fig)
